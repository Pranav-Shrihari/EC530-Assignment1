import csv
from math import radians, sin, cos, acos
from typing import List, Tuple, Dict, Optional

# Earth's radius in kilometers
EARTH_RADIUS_KM = 6371.0

def spherical_distance(
    lat1: float, lon1: float, lat2: float, lon2: float
) -> float:
    """
    Calculate the great‐circle distance between two points
    on Earth specified in decimal degrees.
    """
    φ1, λ1 = radians(lat1), radians(lon1)
    φ2, λ2 = radians(lat2), radians(lon2)
    # law of cosines
    central_angle = acos(
        sin(φ1) * sin(φ2) +
        cos(φ1) * cos(φ2) * cos(λ2 - λ1)
    )
    return EARTH_RADIUS_KM * central_angle

def find_closest_points(
    points: List[Tuple[float, float]],
    candidates: List[Tuple[float, float]]
) -> Dict[Tuple[float, float], Tuple[float, float]]:
    """
    For each (lat, lon) in points, find the closest one in candidates.
    Uses latitude‐sorting + a simple prune to speed up the search.
    """
    # sort candidates by latitude for early break
    candidates_sorted = sorted(candidates, key=lambda p: p[0])
    result: Dict[Tuple[float, float], Tuple[float, float]] = {}

    for lat, lon in points:
        best_dist = float('inf')
        best_pt: Optional[Tuple[float, float]] = None
        # threshold in degrees (~111 km per degree latitude)
        thresh_deg = best_dist / 111

        for cand_lat, cand_lon in candidates_sorted:
            if abs(cand_lat - lat) > thresh_deg:
                break  # no closer candidates beyond this
            d = spherical_distance(lat, lon, cand_lat, cand_lon)
            if d < best_dist:
                best_dist = d
                best_pt = (cand_lat, cand_lon)
                thresh_deg = best_dist / 111

        result[(lat, lon)] = best_pt if best_pt else (None, None)

    return result

def read_csv_coordinates(
    filepath: str, lat_field: str, lon_field: str
) -> List[Tuple[float, float]]:
    """
    Read a CSV and return a list of (lat, lon) tuples.
    """
    coords: List[Tuple[float, float]] = []
    try:
        with open(filepath, newline='', encoding='utf-8') as f:
            reader = csv.DictReader(f)
            for row in reader:
                try:
                    coords.append(
                        (float(row[lat_field]), float(row[lon_field]))
                    )
                except (KeyError, ValueError):
                    # skip rows with missing or invalid data
                    continue
    except FileNotFoundError:
        return []
    except Exception as e:
        print(f"Error reading '{filepath}': {e}")
    return coords

def prompt_for_points() -> List[Tuple[float, float]]:
    """
    Ask the user to either load points from CSV or enter them manually.
    """
    choice = input("Load points from CSV? (y/n): ").strip().lower()
    if choice == 'y':
        path = input("CSV filename: ").strip()
        lat_col = input("Latitude column name: ").strip()
        lon_col = input("Longitude column name: ").strip()
        return read_csv_coordinates(path, lat_col, lon_col)

    if choice == 'n':
        print("Enter points as 'lat,lon' one per line. Type 'done' when finished.")
        pts: List[Tuple[float, float]] = []
        while True:
            line = input("> ").strip()
            if line.lower() == 'done':
                break
            try:
                lat_str, lon_str = line.split(',')
                pts.append((float(lat_str), float(lon_str)))
            except ValueError:
                print("Invalid format. Use e.g. 37.77,-122.42")
        return pts

    print("Invalid choice; no points loaded.")
    return []

def dms_to_decimal(
    degrees: float, minutes: float, seconds: float, direction: str
) -> float:
    """
    Convert DMS (deg/min/sec) to decimal degrees.
    Direction: 'N','E' positive; 'S','W' negative.
    """
    dec = degrees + minutes / 60 + seconds / 3600
    if direction.upper() in ('S', 'W'):
        dec = -dec
    return dec

def main() -> None:
    print("🔍 Match each point in the first set to its nearest neighbor in the second.\n")

    print("First set of points:")
    set1 = prompt_for_points()
    if not set1:
        print("No points provided for set 1. Exiting.")
        return

    print("\nSecond set of points:")
    set2 = prompt_for_points()
    if not set2:
        print("No points provided for set 2. Exiting.")
        return

    matches = find_closest_points(set1, set2)
    for src, tgt in matches.items():
        print(f"{src} → {tgt}")

if __name__ == "__main__":
    main()
