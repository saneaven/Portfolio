# Movie Revenue Prediction

*A regression study that predicts a film's box-office revenue from the metadata it carries before it opens, and asks what a neural network really buys you over a linear baseline.*

## Overview

Revenue is treated here as something learnable from the data a film carries before release: budget, vote counts, popularity, genre, language, release timing, and its synopsis. The project pulls together noisy movie metadata, reframes it as a supervised regression problem, and compares three models (a multi-layer perceptron, the same network extended with text embeddings of the overview, and a plain linear regression baseline) across both a standard test set and a benchmark set carrying the extreme revenue values that get filtered out elsewhere.

The interesting result is not a single best score. Embeddings push raw accuracy up but bring dimensional bloat and clear overfitting, while the linear model is the most robust when the test data gets extreme. The comparison itself is the point.

This is a **solo** project: data cleaning, feature engineering, model building, hyperparameter tuning and evaluation are all my own work. It was built as a machine-learning course project at DigiPen.

## Highlights

- **From raw, multi-source data to a clean regression target.** Aggregated TMDB / IMDB movie metadata is heavily cleaned: dropping high-null or irrelevant columns (taglines, keywords, IMDB id, original-language title), de-duplicating, and removing non-positive / nonsensical values. Filtering on positive revenue alone removes roughly 96% of rows, leaving on the order of ten thousand usable samples.
- **Feature engineering across four data types.** Categorical, cyclic-temporal, text and numeric features are each encoded deliberately rather than thrown in raw.
- **A controlled three-way model comparison.** A base MLP, an embedding-extended MLP, and a linear baseline are evaluated on the same inputs with R², adjusted R², RMSE and MAE, on both a standard split and an outlier-heavy benchmark.
- **An architecture and learning-rate search.** The network shape (neuron count and hidden-layer depth) and the learning rate are swept to find good configurations, with the trade-offs documented as the results came in.
- **An honest read of the trade-off.** Budget and vote count carry the strongest individual signal (r ≈ 0.74 and 0.75 against revenue); on the standard test set the embedding model only just edges the base network (R² ≈ 0.71 vs 0.70) yet is penalised hard on adjusted R² and overfits, while on the outlier-heavy set the linear model is the most robust: best adjusted R² and the slope closest to ideal on the extreme-revenue films.

## Feature engineering

| Feature group | Encoding | Notes |
| --- | --- | --- |
| Genres, spoken / original language | Multi-hot into `[-1, 1]` | Rare categories below a count threshold collapsed into an `other` bucket; a film can carry several genres at once with no false ordering. |
| Release date | Year scaled to `[-1, 1]`; month / day sine-cosine encoded | Cyclic encoding so December sits next to January rather than at the far end of a line. |
| Overview (synopsis) | Pre-trained sentence embeddings | Produced with the `all-MiniLM-L6-v2` Sentence Transformer; forms the "extended" feature set tested against the version without it. |
| Budget, vote count, popularity | Standardised | Scaled before training; budget and vote count showed the strongest individual correlation with revenue. |

## Models & results

All three models predict the same (scaled) revenue target from the engineered feature vector, split 80/20 into train and test.

- **Extended MLP:** a deep `MLPRegressor` (six hidden layers of 750 units, ReLU, Adam, early stopping) plus the overview embeddings posted the highest raw fit on the standard test set (R² ≈ 0.71), but only barely ahead of the base model, and it fit the training data far more tightly (train R² ≈ 0.84 vs test R² ≈ 0.71), the clear overfitting the project set out to expose.
- **Base MLP:** the tuned network without embeddings (eight hidden layers of 130 units) landed close behind on the standard test set (R² ≈ 0.70) with a far smaller feature space, so its adjusted R² held up much better than the extended model's.
- **Linear baseline:** plain `LinearRegression` came in around R² ≈ 0.63 on the standard test set. On the outlier-heavy benchmark it was the most robust of the three: its raw R² (≈ 0.60) sat just behind the base MLP (≈ 0.60), but it took by far the smallest adjusted-R² penalty and drew the prediction slope closest to ideal on the extreme-revenue films.

A separate hyperparameter study swept hidden-layer depth and neuron count (from ten up to several hundred per layer, and into the high hundreds for the embedding-extended model) and the learning rate, finding that an initial learning rate of `1e-05` trained better than `1e-06`, that ReLU and Adam outperformed the alternatives tried, and that adding capacity mostly demanded more iterations to converge while validation R² plateaued near 0.7.

## How it works

The whole pipeline lives in a single annotated Jupyter notebook and runs top to bottom:

1. **Load & explore** the TMDB dataset, inspecting nulls, distributions and correlations.
2. **Clean:** drop irrelevant / high-null columns, de-duplicate, and remove non-positive or out-of-range values.
3. **Encode:** multi-hot categoricals, cyclic dates, optional overview embeddings, and standard-scaled numerics, concatenated into one feature vector.
4. **Split** into train / test, plus a benchmark set holding the extreme revenue values.
5. **Train & tune** the MLP via an architecture and hyperparameter search; train the linear baseline on the same features.
6. **Evaluate & compare** all three models with R² / adjusted R², RMSE and MAE, and scatter plots of predicted vs. actual revenue, on both standard and outlier-heavy data.

## Tech stack

- **Language:** Python (Jupyter notebook)
- **Modelling:** scikit-learn: `MLPRegressor`, `LinearRegression`, early stopping
- **Text embeddings:** `sentence-transformers` (`all-MiniLM-L6-v2`)
- **Data & features:** pandas, numpy, `StandardScaler`, `MinMaxScaler`, `MultiLabelBinarizer`
- **Evaluation & plots:** matplotlib, seaborn

## Build & run

The project is a Jupyter notebook, not a packaged application.

1. Install the dependencies: `pandas`, `numpy`, `scikit-learn`, `sentence-transformers`, `matplotlib`, `seaborn`, and `jupyter`.
2. Place the TMDB movie dataset CSV at `./data/TMDB_movie_dataset_v11.csv` (the dataset itself is not committed; sources are linked in the notebook).
3. Open `project_moviemovie_minjae.ipynb` in Jupyter and run the cells top to bottom.

The full hyperparameter sweeps are commented out by default and their outputs are cached as CSVs under `result/`, so the notebook can reproduce the analysis without rerunning the multi-hour search.

## Project structure

```
Movie Revenue Prediction/
├── project_moviemovie_minjae.ipynb   # main notebook: cleaning -> features -> models -> evaluation
├── project_moviemovie.ipynb          # earlier working copy of the notebook
├── playground.ipynb                  # scratch experiments
├── result/                           # cached hyperparameter-search results (CSV)
│   ├── mlp_regressor_results.csv
│   ├── mlp_regressor_results_extended.csv
│   └── mlp_regressor_results_pruned_extended*.csv
└── Plots/                            # exported figures
```

## Links

- [Source on GitHub](https://github.com/saneaven/Portfolio/tree/main/projects/ai/Movie%20Revenue%20Prediction)

## Status

Solo machine-learning course project at DigiPen: a complete study from raw, multi-source data to model comparison and write-up.
