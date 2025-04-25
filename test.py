import unittest
import tempfile
import os
import csv

from assignment1 import (
    spherical_distance,
    find_closest_points,
    dms_to_decimal,
    read_csv_coordinates
)

class TestLocationUtils(unittest.TestCase):
    def test_spherical_distance_zero(self):
        # Same point distance is zero
        self.assertAlmostEqual(spherical_distance(0, 0, 0, 0), 0.0, places=6)

    def test_spherical_distance_known(self):
        # Approximate distance: 1° longitude at equator ~111.1949 km
        dist = spherical_distance(0, 0, 0, 1)
        self.assertAlmostEqual(dist, 111.1949, places=3)

    def test_dms_to_decimal(self):
        # North/East positive, South/West negative
        self.assertEqual(dms_to_decimal(30, 0, 0, 'N'), 30.0)
        self.assertEqual(dms_to_decimal(30, 30, 0, 'W'), -(30 + 30/60))
        self.assertEqual(dms_to_decimal(0, 0, 30, 'S'), -(30/3600))

    def test_find_closest_points(self):
        pts = [(0, 0), (10, 10)]
        cands = [(0, 1), (9, 9), (20, 20)]
        result = find_closest_points(pts, cands)
        expected = {
            (0, 0): (0, 1),
            (10, 10): (9, 9)
        }
        self.assertEqual(result, expected)

    def test_read_csv_coordinates(self):
        header = ['lat_field', 'lon_field', 'other']
        rows = [
            {'lat_field': '1.1', 'lon_field': '2.2', 'other': 'x'},
            {'lat_field': '3.3', 'lon_field': '4.4', 'other': 'y'},
            {'lat_field': 'bad', 'lon_field': '5.5', 'other': 'z'},  # invalid lat
        ]
        with tempfile.NamedTemporaryFile('w+', newline='', delete=False) as tmp:
            writer = csv.DictWriter(tmp, fieldnames=header)
            writer.writeheader()
            for row in rows:
                writer.writerow(row)
            tmp.flush()
            path = tmp.name

        try:
            coords = read_csv_coordinates(path, 'lat_field', 'lon_field')
            self.assertEqual(coords, [(1.1, 2.2), (3.3, 4.4)])
        finally:
            os.remove(path)

    def test_read_csv_file_not_found(self):
        # Non-existent file should return empty list without error
        coords = read_csv_coordinates('no_such_file.csv', 'lat', 'lon')
        self.assertEqual(coords, [])

if __name__ == '__main__':
    unittest.main()
