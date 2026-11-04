# NYC Taxi Analysis

*An exploratory analysis of roughly 15 million 2013 New York yellow-taxi trips: joining trip records to their fares and digging for patterns in where rides end, when the city speeds up and slows down, how people pay, and what really drives a day's revenue.*

## Overview

The project starts from two large public datasets: trip records and the matching fare records for New York yellow taxis. Each pair of files is merged in pandas into a single table that links every ride's geography and timing to what it cost, so the city's movement and its economics can be read side by side.

The first month alone holds **14,776,615 rows** per file, and the analysis works across that full scale rather than a toy sample. Before any patterns are drawn, the data is cleaned hard: dropoffs with null or out-of-bounds coordinates are filtered out so the conclusions come from rides that actually make sense rather than logging glitches.

None of the findings need a predictive model: they fall out of careful joining, cleaning, and the right plot.

## My Role

Solo project, end to end. I did the data wrangling, merging and cleaning, all of the analyses, and every visualization. This was built as a data-analysis course project at DigiPen.

## Highlights

- **Where rides end.** A heatmap of 500,000 sampled dropoffs is rendered over the Google Maps API via `gmaps`. Dropoffs spread fairly evenly across Manhattan, with a clear hotspot around the 7th/8th Avenue corridor near Madison Square Garden.
- **When the city is quickest.** Plotting seconds-per-mile across the 24-hour cycle shows traffic at its freest around 5 a.m.: the hour where the same trip covers ground fastest.
- **How people pay.** Even in 2013, card already edged out cash: about **52.4%** of trips paid by card versus **47.3%** cash. The MTA tax turns out to be a flat, fixed cost, uncorrelated with revenue rather than scaling with it as a percentage tax would.
- **Short hops.** Trip distance follows a left-skewed distribution clustered around two miles; extreme outliers are trimmed so the bulk of the data is visible.
- **What drives a day.** Through monthly and daily aggregation plus min-max normalization, daily total revenue tracks the **number of rides** far more closely than it tracks the average fare per trip: earnings are a volume game, not a price one. Weekday takings also beat weekends, against the tourism hypothesis.

## How It Works

The notebook moves through a single linear narrative:

1. **Load and type.** `trip_data_1.csv` and `trip_fare_1.csv` are read with pandas; the datetime columns are converted to real `datetime` types so they can drive time-based plots.
2. **Geospatial.** Dropoff coordinates are bounded to a sensible NYC box (longitude -75…-73, latitude 40…41, excluding zeros), sampled down to 500,000 points, and pushed into a `gmaps` heatmap layer.
3. **Merge and correlate.** The fare and trip tables are joined on `medallion` + `pickup_datetime`, and a correlation matrix over the numeric columns (styled with a custom matplotlib colormap) confirms the strong link between total fare, trip time and distance, and the flat behaviour of the MTA tax.
4. **Payment and distance.** `value_counts` on the payment type drives a pie chart; a histogram (with outliers folded down) shows the distance distribution.
5. **Time of day.** A derived seconds-per-mile column is scatter-plotted against pickup time of day to surface the 5 a.m. traffic minimum.
6. **Temporal revenue.** Earnings are aggregated by day of week, by month (sampling 100,000 rows from each of the twelve monthly fare files), and by calendar day. Rolling means smooth the daily series, and min-max normalization lets total earning, average earning and ride count be compared on one chart.

## Tech Stack

- **Language:** Python (Jupyter notebook)
- **Wrangling:** pandas
- **Visualization:** matplotlib, `gmaps` (Google Maps heatmaps)
- **Config:** `python-dotenv` for the Google Maps API key
- **Data:** NYC TLC 2013 yellow-taxi trip + fare CSVs (~14.8M rows in the first monthly file)

## Build & Run

The analysis lives in a single Jupyter notebook, `FinalProject_Minjae_Kyung.ipynb`.

1. Install the dependencies:

   ```bash
   pip install pandas matplotlib gmaps python-dotenv jupyter
   ```

2. Enable the notebook widget extensions used by the map (as the notebook does):

   ```bash
   jupyter nbextension enable --py widgetsnbextension
   jupyter nbextension enable --py gmaps
   ```

3. Provide a Google Maps API key. Create a `.env` file in this folder with:

   ```
   MAP_API_KEY=your_key_here
   ```

4. Place the NYC TLC 2013 CSVs alongside the notebook so the expected paths resolve:

   - `trip_data/trip_data_1.csv` … (trip records)
   - `trip_fare/trip_fare_1.csv` … `trip_fare_12.csv` (fare records)

5. Open the notebook in Jupyter and run the cells top to bottom.

> Note: the raw CSVs are large and are not included in the repository. The > heatmap requires a valid Google Maps API key to render.

## Project Structure

```
NY Taxi Analysis/
├── FinalProject_Minjae_Kyung.ipynb   # the full analysis, end to end
├── Presentation.pptx                 # accompanying presentation
└── .env                              # holds MAP_API_KEY (Google Maps API key)
```

## Links

- [Source on GitHub](https://github.com/saneaven/Portfolio/tree/main/projects/ai/NY%20Taxi%20Analysis)

## Context

Solo data-analysis project, built at DigiPen. 2013 NYC TLC yellow-taxi dataset.
