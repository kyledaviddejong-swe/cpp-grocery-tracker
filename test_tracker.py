"""Focused command-line regression checks. Pass the compiled executable path."""
from pathlib import Path
import os
import subprocess
import sys
import tempfile
import unittest

EXECUTABLE = Path(sys.argv.pop(1) if len(sys.argv) > 1 else "grocery-tracker").resolve()
SAMPLE = "Apples\nPears\nBread\nApples\nMilk\nPears\nApples\n"
COUNTS = "Apples 3\nBread 1\nMilk 1\nPears 2\n"


class TrackerChecks(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.folder = Path(self.temp.name)
        (self.folder / "groceries.txt").write_text(SAMPLE)

    def run_tracker(self, commands):
        return subprocess.run([str(EXECUTABLE)], input=commands, text=True,
                              capture_output=True, cwd=self.folder, timeout=5)

    def test_lookup_counts_histogram_and_backup(self):
        result = self.run_tracker("1\nApples\n1\napples\n2\n3\n4\n")
        self.assertEqual(result.returncode, 0)
        for expected in ("Apples 3", "apples 0", COUNTS,
                         "Apples ***\nBread *\nMilk *\nPears **\n"):
            self.assertIn(expected, result.stdout)
        self.assertEqual((self.folder / "frequency.dat").read_text(), COUNTS)

    def test_rejects_partial_menu_numbers(self):
        result = self.run_tracker("4junk\n2 extra\n0\n\n2\n4\n")
        self.assertEqual(result.returncode, 0)
        self.assertEqual(result.stdout.count("Error: Invalid option"), 4)
        self.assertIn(COUNTS, result.stdout)

    def test_eof_at_both_prompts(self):
        for commands in ("", "1\n"):
            with self.subTest(commands=commands):
                result = self.run_tracker(commands)
                self.assertEqual(result.returncode, 0)
                self.assertIn("Input closed", result.stdout)

    def test_rejects_empty_or_multiword_lookup(self):
        result = self.run_tracker("1\n\n1\nApples Pears\n1\nPears\n4\n")
        self.assertEqual(result.returncode, 0)
        self.assertEqual(result.stdout.count("Error: Enter one item name."), 2)
        self.assertIn("Pears 2", result.stdout)

    def test_missing_input(self):
        (self.folder / "groceries.txt").unlink()
        self.assertEqual(self.run_tracker("4\n").returncode, 1)

    def test_backup_open_failure(self):
        (self.folder / "frequency.dat").mkdir()
        self.assertEqual(self.run_tracker("4\n").returncode, 1)

    @unittest.skipUnless(os.name == "posix" and Path("/dev/full").exists(),
                         "Requires the /dev/full write-failure device")
    def test_backup_write_failure(self):
        (self.folder / "frequency.dat").symlink_to("/dev/full")
        result = self.run_tracker("4\n")
        self.assertEqual(result.returncode, 1)
        self.assertIn("Failed to create backup", result.stdout)


if __name__ == "__main__":
    unittest.main()
