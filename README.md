# Cab Nearby Customer Finder

A lightweight, high-performance C++ application designed to parse customer location data, calculate geographical distances using the Great-Circle distance formula, and filter records within a 50km radius.

## Features
- **Manual JSON Parsing:** Extracts attributes (`user_id`, `name`, `latitude`, `longitude`) efficiently using standard C++ string manipulation.
- **Geospatial Calculation:** Implements the Haversine/Great-Circle formula to accurately measure distances on Earth.
- **Proximity Sorting:** Automatically sorts filtered results in ascending order by distance.
- **Robust Exception Handling:** Safely handles file-opening errors and skips malformed data lines.

## Tech Stack
- **Language:** C++ (Standard Template Library)
- **Data Format:** JSON (`customers.json` input, `answer.json` output)

## How to Compile & Run
1. Ensure `cab_book.cpp` and `customers.json` are in the same directory.
2. Compile using a C++ compiler:
   ```bash
   g++ -O3 cab_book.cpp -o cab_book
