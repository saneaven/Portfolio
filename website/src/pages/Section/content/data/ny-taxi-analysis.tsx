import type { ContentBlocks } from '../contentblock/types'
import githubIcon from '../../../../assets/github-white-icon.svg'

const nyTaxiAnalysis: ContentBlocks = [
  {
    type: 'lead',
    eyebrow: 'AI · Data Analysis',
    title: 'NYC Taxi Analysis',
    subtitle:
      'An exploratory analysis of roughly 15 million New York yellow-taxi trips from 2013, joining trip records to their fares and digging for patterns in where rides end, when the city speeds up and slows down, how people pay, and what really drives a day\'s revenue.',
    meta: [
      { label: 'Role', value: 'Solo — data wrangling, analysis & visualization' },
      { label: 'Focus', value: 'Exploratory data analysis · geospatial · time series' },
      { label: 'Stack', value: 'Python · pandas / numpy · matplotlib · gmaps' },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Overview',
    title: 'Join the trips to the fares, then clean hard',
    blocks: [
      {
        type: 'text',
        paragraphs: [
          'The starting point is two large CSV datasets — trip records and the matching fare records, around 14.8 million rows across 2013 — merged in pandas into one table linking each ride\'s geography and timing to what it cost.',
          'Real-world data needs real cleaning: trips with null or out-of-bounds coordinates are filtered out before any analysis, so the patterns that follow come from rides that actually make sense rather than logging glitches.',
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Core — What the data showed',
    title: 'Geography, time and money',
    blocks: [
      {
        type: 'minis',
        items: [
          {
            title: 'Where rides end',
            body: 'A heatmap of half a million dropoffs rendered through the Google Maps API shows a fairly even spread across Manhattan, with a clear hotspot around Madison Square Garden.',
          },
          {
            title: 'When the city is quickest',
            body: 'Tracking seconds-per-mile across the 24-hour cycle, traffic is at its freest around 5 a.m. — the hour where the same trip covers ground fastest.',
          },
          {
            title: 'How people pay',
            body: 'Even in 2013, card already edged out cash — roughly 52.5% of trips paid by card versus 47.3% cash — and the fixed MTA tax shows up as a flat cost uncorrelated with revenue.',
          },
          {
            title: 'What drives a day',
            body: 'Daily revenue tracks the number of rides almost perfectly (r ≈ 0.99) and only loosely follows the average fare per trip — earnings are a volume game, not a price one. Weekday takings also beat weekends, against the tourism hypothesis.',
          },
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Takeaways',
    title: 'The story the trips tell',
    blocks: [
      {
        type: 'text',
        paragraphs: [
          'Put together, the picture is a city of short hops — trip distance clusters around two miles — where the route to a good day is more rides rather than pricier ones, card payment was already the norm a decade ago, and demand stays remarkably steady through the year with only mild seasonal drift. None of it needs a model; it falls out of careful joining, cleaning and the right plot.',
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
          { layer: 'Data', items: ['NYC TLC 2013', 'Trip + fare CSVs', '~14.8M rows'] },
          { layer: 'Wrangling', items: ['pandas', 'numpy', 'Merge & clean'] },
          { layer: 'Visualization', items: ['matplotlib', 'gmaps heatmap', 'Google Maps API'] },
          { layer: 'Environment', items: ['Python', 'Jupyter'] },
        ],
      },
    ],
  },
  {
    type: 'links',
    links: [
      {
        label: 'Source on GitHub',
        href: 'https://github.com/saneaven/Portfolio/tree/main/projects/ai/NY%20Taxi%20Analysis',
        icon: githubIcon,
        hint: 'Code & technical docs',
      },
    ],
  },
  {
    type: 'closing',
    text: 'A solo exploratory analysis end to end — merging and cleaning ~15 million 2013 taxi trips, then reading their geography, timing and economics to show a short-hop, card-paying, volume-driven city.',
  },
]

export default nyTaxiAnalysis
