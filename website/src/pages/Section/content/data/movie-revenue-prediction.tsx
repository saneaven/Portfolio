import type { ContentBlocks } from '../contentblock/types'
import githubIcon from '../../../../assets/github-white-icon.svg'
import MovieRevenueArchDiagram from './movie-revenue-prediction.diagram'

const movieRevenuePrediction: ContentBlocks = [
  {
    type: 'lead',
    eyebrow: 'AI · Machine Learning',
    title: 'Movie Revenue Prediction',
    subtitle:
      'A regression study that treats revenue as something learnable from the data a film carries before it opens — budget, cast votes, genre, language, release timing and its synopsis. A multi-layer perceptron is trained on engineered features, extended with text embeddings of the overview, and measured against a linear baseline on both a standard and an outlier-heavy test set.',
    meta: [
      { label: 'Role', value: 'Solo — data, feature engineering, modelling & evaluation' },
      { label: 'Focus', value: 'Regression · feature engineering · model comparison' },
      { label: 'Stack', value: 'Python · scikit-learn · pandas / numpy · matplotlib / seaborn' },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Overview',
    title: 'Turn messy movie metadata into a regression target',
    blocks: [
      {
        type: 'text',
        paragraphs: [
          'The data is pulled together from TMDB and IMDB sources and is noisy out of the box, so the first job is aggressive cleaning: dropping high-null or irrelevant columns (poster paths, taglines, homepages) and removing rows with missing values while keeping as many samples as possible.',
          'What survives is reframed as a supervised regression problem — predict revenue from a mix of numeric, categorical, temporal and text features — split 80/20 into train and test, with a separate benchmark set carrying extreme revenue values to probe how the models behave on outliers.',
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Core — Feature engineering',
    title: 'Encoding everything a film tells you up front',
    blocks: [
      {
        type: 'cardGrid',
        cards: [
          {
            title: 'Categorical multi-hot',
            body: 'Genres, spoken languages and original language are multi-hot encoded and normalised into a [-1, 1] range, so a film can carry several genres at once without imposing a false ordering.',
          },
          {
            title: 'Cyclic dates',
            body: 'The release date is decomposed into year, month, day and day-of-week, with the cyclical parts sine/cosine encoded so December sits next to January rather than at the opposite end of a line.',
          },
          {
            title: 'Text embeddings',
            body: 'The title and overview are turned into pre-trained word embeddings, letting the synopsis contribute signal — an "extended" feature set tested directly against the version without it.',
          },
          {
            title: 'Scaled numerics',
            body: 'Budget, vote count and popularity are standardised and scaled; of all the inputs, budget and vote count showed the strongest individual correlation with revenue (r ≈ 0.74 and 0.75).',
          },
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Models & results',
    title: 'A neural net, its embedding variant, and a linear baseline',
    blocks: [
      {
        type: 'custom',
        render: () => <MovieRevenueArchDiagram />,
      },
      {
        type: 'minis',
        items: [
          {
            title: 'Extended MLP',
            body: 'A three-layer MLP regressor (100 units each, ReLU, Adam, early stopping) with the overview embeddings reached the best fit on the standard set — R² ≈ 0.84 — at the cost of clear overfitting.',
          },
          {
            title: 'Base MLP',
            body: 'The same network without embeddings landed at R² ≈ 0.75 — strong, and with a much smaller feature space, so its adjusted R² held up better than the extended model\'s.',
          },
          {
            title: 'Linear baseline',
            body: 'Plain linear regression came in at R² ≈ 0.66 on the standard set but proved the most robust of the three on the outlier-heavy benchmark, generalising where the networks did not.',
          },
          {
            title: 'The honest read',
            body: 'Embeddings bought accuracy but also dimensional bloat and overfitting; the takeaway is the trade-off itself — more signal is not free, and the simplest model wins when the test data gets extreme.',
          },
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Tuning',
    title: 'Searching the architecture and learning rate',
    blocks: [
      {
        type: 'text',
        paragraphs: [
          'Beyond the headline comparison, the study sweeps the network shape — neuron counts from ten up to a couple hundred and hidden-layer depth across a wide range — and the learning rate, finding that an initial rate of 1e-05 trained better than 1e-06, and that adding capacity mostly just demanded more iterations to converge while validation R² plateaued near 0.7.',
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Stack',
    title: 'What it\'s built on',
    blocks: [
      {
        type: 'stack',
        rows: [
          { layer: 'Modelling', items: ['scikit-learn', 'MLPRegressor', 'LinearRegression', 'Early stopping'] },
          { layer: 'Features', items: ['Multi-hot encoding', 'Cyclic date encoding', 'Word embeddings', 'StandardScaler / MinMaxScaler'] },
          { layer: 'Data', items: ['TMDB', 'IMDB', 'pandas', 'numpy'] },
          { layer: 'Evaluation', items: ['R² / adjusted R²', 'RMSE / MAE', 'matplotlib', 'seaborn'] },
        ],
      },
    ],
  },
  {
    type: 'links',
    links: [
      {
        label: 'Source on GitHub',
        href: 'https://github.com/saneaven/Portfolio/tree/main/projects/ai/Movie%20Revenue%20Prediction',
        icon: githubIcon,
        hint: 'Code & technical docs',
      },
    ],
  },
  {
    type: 'closing',
    text: 'A solo regression study from raw data to result — cleaning multi-source movie metadata, engineering categorical, cyclic, text-embedding and numeric features, and comparing an MLP (R² ≈ 0.84) against a more robust linear baseline on standard and outlier-heavy data.',
  },
]

export default movieRevenuePrediction
