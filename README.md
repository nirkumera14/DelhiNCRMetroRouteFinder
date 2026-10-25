# Delhi-NCR Metro Route Finder (C++)

A command-line project that uses **Dijkstra's shortest-path algorithm** to find a route between metro stations in the bundled Delhi-NCR sample network.

## Project files

After extracting `DelhiNCRMetroRouteFinder.zip`, you should see:

```text
DelhiNCRMetroRouteFinder/
├── main.cpp          # C++ source code and Dijkstra implementation
├── metro_lines.txt   # Metro line and station-sequence data
└── README.md         # This guide
```

**Keep `main.cpp` and `metro_lines.txt` in the same folder.** The program loads `metro_lines.txt` when it starts.

## Requirements

- Windows 10/11, Linux, or macOS
- A C++ compiler that supports C++17 (`g++` recommended)
- Visual Studio Code is recommended but optional

### Install a compiler on Windows

If `g++ --version` does not work:
1. Install MSYS2 from <https://www.msys2.org/>.
2. Open the **MSYS2 UCRT64** terminal.
3. Install GCC:

   ```bash
   pacman -S --needed mingw-w64-ucrt-x86_64-gcc
   ```

4. Add `C:\msys64\ucrt64\bin` to your Windows `Path` environment variable.
5. Close and reopen VS Code, then verify:

   ```powershell
   g++ --version
   ```

## Compile and run in VS Code (Windows)

1. Download the ZIP and choose **Extract All**.
2. Open VS Code.
3. Choose **File → Open Folder** and select the extracted `DelhiNCRMetroRouteFinder` folder.
4. Choose **Terminal → New Terminal**.
5. Check that the terminal is in the project folder and that both files are present:

   ```powershell
   dir
   ```

6. Compile the program:

   ```powershell
   g++ -std=c++17 -O2 -Wall -Wextra main.cpp -o metro.exe
   ```

7. Run it:

   ```powershell
   .\metro.exe
   ```

Compile again after changing `main.cpp`. You do not need to recompile if you only change `metro_lines.txt`.

## Compile and run on Linux or macOS

Open a terminal in the project folder and run:

```bash
g++ -std=c++17 -O2 -Wall -Wextra main.cpp -o metro
./metro
```

## How to use the program

When the program starts, it shows a menu:

```text
1. Find shortest route
2. List all stations
3. List metro lines
4. Exit
Choice:
```

### Find a route

1. Enter `1` and press Enter.
2. Type the source station exactly as it appears in the station list (capitalization is ignored).
3. Type the destination station.
4. Read the route, number of station-to-station segments, and approximate line changes.

Example:

```text
Choice: 1
Source station: Rajiv Chowk
Destination station: Botanical Garden
```

### List stations or lines

- Enter `2` to print station names loaded from the dataset.
- Enter `3` to see the metro line names and how many stops are listed on each.
- Enter `4` to exit.

## How the graph is represented

The graph is stored as an **adjacency list** in C++:

```cpp
vector<vector<Edge>> graph;
```

Each station has a numeric ID. `stationIds` maps normalized station names to IDs. Each edge stores the neighbor station ID, a weight, and the line name. Adjacent stations in each line are connected in both directions. Shared station names are represented as shared graph vertices, enabling interchange connections where names match.

Dijkstra's algorithm uses a priority queue and a `parent` array to reconstruct the shortest route.

## What “shortest” means in this version

Every adjacent-station edge currently has weight `1`. Dijkstra therefore minimizes the number of station-to-station segments. It does **not** calculate actual kilometres, fares, live service status, timetable-based travel time, or the walking time required for interchanges.

## Data coverage and accuracy note

The bundled dataset includes major line sequences for the DMRC network, the Noida-Greater Noida Aqua Line, and Gurugram Rapid Metro. It is an **educational dataset and has not been fully verified as a complete, authoritative live network**. Station names, newer extensions, branches, and interchange links may need correction. Check official maps before using results to plan a real journey.

Reference websites:
- DMRC: <https://delhimetrorail.com/>
- Noida Metro Rail Corporation: <https://www.nmrcnoida.com/StationList>
- Delhi Metro Rapid Metro information: <https://new.delhimetrorail.com/en/rapid-metro>

## Troubleshooting

### `g++ is not recognized`
Install a compiler and make sure its `bin` directory is in your Windows `Path`. Reopen VS Code after changing `Path`.

### `metro_lines.txt` could not be loaded
Run `metro.exe` from the project folder, where `metro_lines.txt` is located. Do not move the executable away from the data file.

### `Source station not found`
Choose option `2` and copy a station name from the printed list.

### `metro.exe` is not found
Compile first and confirm the compile command finished without errors. Then run `.\metro.exe` in PowerShell.

## License / contributions

This is a learning project. You can extend it by validating station data, adding actual distance or time weights, modelling interchange walking links, adding unit tests, or building a graphical interface.
