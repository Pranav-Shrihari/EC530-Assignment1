import csv
import pytest
from assignment1 import (
    spherical_distance,
    find_closest_points,
    dms_to_decimal,
    read_csv_coordinates
)


def test_spherical_distance_zero():
    """Same point should yield zero distance."""
    assert spherical_distance(0, 0, 0, 0) == pytest.approx(0.0, abs=1e-6)


def test_spherical_distance_known():
    """1° longitude at equator is about 111.1949 km."""
    dist = spherical_distance(0, 0, 0, 1)
    assert dist == pytest.approx(111.1949, rel=1e-3)


def test_dms_to_decimal():
    """DMS conversions for N/E positive, S/W negative."""
    assert dms_to_decimal(30, 0, 0, 'N') == 30.0
    assert dms_to_decimal(30, 30, 0, 'W') == pytest.approx(-(30 + 30/60))
    assert dms_to_decimal(0, 0, 30, 'S') == pytest.approx(-(30/3600))


def test_find_closest_points():
    pts = [(0, 0), (10, 10)]
    cands = [(0, 1), (9, 9), (20, 20)]
    result = find_closest_points(pts, cands)
    expected = {
        (0, 0): (0, 1),
        (10, 10): (9, 9)
    }
    assert result == expected


def test_read_csv_coordinates(tmp_path):
    """Valid and invalid rows in CSV produce correct list of coords."""
    header = ['lat_field', 'lon_field', 'other']
    rows = [
        {'lat_field': '1.1', 'lon_field': '2.2', 'other': 'x'},
        {'lat_field': '3.3', 'lon_field': '4.4', 'other': 'y'},
        {'lat_field': 'bad', 'lon_field': '5.5', 'other': 'z'},
    ]
    file_path = tmp_path / "coords.csv"
    with open(file_path, 'w', newline='') as f:
        writer = csv.DictWriter(f, fieldnames=header)
        writer.writeheader()
        for row in rows:
            writer.writerow(row)

    coords = read_csv_coordinates(str(file_path), 'lat_field', 'lon_field')
    assert coords == [(1.1, 2.2), (3.3, 4.4)]


def test_read_csv_file_not_found():
    """Nonexistent file returns empty list without raising."""
    coords = read_csv_coordinates('no_such_file.csv', 'lat', 'lon')
    assert coords == []