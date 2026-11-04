import dummy from '../assets/section/dummy.avif'
import novelBudsHero from '../assets/section/ai/novelbuds/hero.avif'
import raonVerseHero from '../assets/section/ai/raonverse/hero.avif'
import nextGenDynamicsHero from '../assets/section/ai/nextgendynamics/hero.avif'
import otherProjectsHero from '../assets/section/ai/others/hero.avif'
import qByNoflexHero from '../assets/section/game/q-by-noflex/hero.avif'
import anOmenHero from '../assets/section/game/an-omen-in-the-mirror/hero.avif'
import type { ContentBlocks } from '../pages/Section/content/contentblock/types'
import raonVerse from '../pages/Section/content/data/raon-verse'
import novelBuds from '../pages/Section/content/data/novel-buds'
import novelBudsAgentModel from '../pages/Section/content/data/novelbuds-agent-model'
import nextGenDynamics from '../pages/Section/content/data/next-gen-dynamics'
import buzzamaker from '../pages/Section/content/data/buzzamaker'
import jamoTokenizationTranslator from '../pages/Section/content/data/jamo-tokenization-translator'
import generativeAi from '../pages/Section/content/data/generative-ai'
import deepLearning from '../pages/Section/content/data/deep-learning'
import llmCardGame from '../pages/Section/content/data/llm-card-game'
import movieRevenuePrediction from '../pages/Section/content/data/movie-revenue-prediction'
import nyTaxiAnalysis from '../pages/Section/content/data/ny-taxi-analysis'
import qByNoflex from '../pages/Section/content/data/q-by-noflex'
import anOmenInTheMirror from '../pages/Section/content/data/an-omen-in-the-mirror'
import gameAi from '../pages/Section/content/data/game-ai'
import theForestOfFood from '../pages/Section/content/data/the-forest-of-food'
import bounceNds from '../pages/Section/content/data/bounce-nds'
import painter from '../pages/Section/content/data/painter'
import sonarSoul from '../pages/Section/content/data/sonar-soul'
import dragonswordLegend from '../pages/Section/content/data/dragonsword-legend'

export type CategoryKey = 'ai' | 'game'

export type Project = {
  slug: string // unique within its category
  title: string
  subtitle: string
  image: string
  blocks: ContentBlocks
  // Hubs render the child selected by the /<child-slug> segment instead of blocks.
  children?: Project[]
}

export type Category = {
  key: CategoryKey
  name: string
  gradient: { from: string; to: string }
  projects: Project[] // order shared by PartLanding and the Section menu
}

export const CATEGORIES: Record<CategoryKey, Category> = {
  ai: {
    key: 'ai',
    name: 'Artificial Intelligence',
    gradient: { from: 'var(--ai-from)', to: 'var(--ai-to)' }, // blue-deep → green-deep
    projects: [
      { slug: 'raon-verse', title: 'Raon:Verse', subtitle: 'Multiverse stories built together with agents', image: raonVerseHero, blocks: raonVerse },
      { slug: 'novel-buds', title: 'Novel Buds', subtitle: 'Novel management and writing agent', image: novelBudsHero, blocks: novelBuds },
      { slug: 'next-gen-dynamics', title: 'Next Gen Dynamics', subtitle: 'Spider bot reinforcement learning project', image: nextGenDynamicsHero, blocks: nextGenDynamics },
      {
        slug: 'other-projects',
        title: 'Other Projects',
        subtitle: 'Other interesting projects',
        image: otherProjectsHero,
        blocks: [], // hub mode renders its children instead of its own body
        children: [
          { slug: 'novelbuds-agent-model', title: 'Novel Buds Agent', subtitle: 'Fine-tuning Gemma 4 31B into a novel-writing agent', image: dummy, blocks: novelBudsAgentModel },
          { slug: 'buzzamaker', title: 'BuzzaMaker', subtitle: 'RL portfolio generation from stock & news', image: dummy, blocks: buzzamaker },
          { slug: 'jamo-tokenization-translator', title: 'Jamo Tokenization Translator', subtitle: 'Jamo-based Korean tokenization & translation', image: dummy, blocks: jamoTokenizationTranslator },
          { slug: 'generative-ai', title: 'Generative AI', subtitle: 'Decoding, agents, ControlNet & QLoRA fine-tuning', image: dummy, blocks: generativeAi },
          { slug: 'deep-learning', title: 'Deep Learning', subtitle: 'Nets from scratch, CNN transfer learning & Double DQN', image: dummy, blocks: deepLearning },
          { slug: 'llm-card-game', title: 'LLM Card Game', subtitle: 'Gemini-driven turn-based card battler', image: dummy, blocks: llmCardGame },
          { slug: 'movie-revenue-prediction', title: 'Movie Revenue Prediction', subtitle: 'Predicting box-office revenue with an MLP', image: dummy, blocks: movieRevenuePrediction },
          { slug: 'ny-taxi-analysis', title: 'NYC Taxi Analysis', subtitle: 'EDA across ~15M 2013 NYC taxi trips', image: dummy, blocks: nyTaxiAnalysis },
        ],
      },
    ],
  },
  game: {
    key: 'game',
    name: 'Game Programming',
    gradient: { from: 'var(--game-from)', to: 'var(--game-to)' }, // gold → crimson
    projects: [
      { slug: 'q-by-noflex', title: 'Q by Noflex', subtitle: 'Roguelike platformer on a custom C++/ECS engine', image: qByNoflexHero, blocks: qByNoflex },
      { slug: 'an-omen-in-the-mirror', title: 'An Omen In The Mirror', subtitle: 'UI/UX design & implementation for a narrative puzzle game', image: anOmenHero, blocks: anOmenInTheMirror },
      {
        slug: 'other-projects',
        title: 'Other Projects',
        subtitle: 'Other interesting projects',
        image: dummy,
        blocks: [], // hub mode renders its children instead of its own body
        children: [
          { slug: 'game-ai', title: 'Game AI Projects', subtitle: 'Behavior trees, A* pathfinding & terrain analysis', image: dummy, blocks: gameAi },
          { slug: 'the-forest-of-food', title: 'The Forest of Food', subtitle: 'Seasonal cooking adventure on the doodle engine', image: dummy, blocks: theForestOfFood },
          { slug: 'bounce-nds', title: 'Bounce NDS', subtitle: 'Physics ball-platformer in JavaScript / p5.js', image: dummy, blocks: bounceNds },
          { slug: 'painter', title: 'Painter', subtitle: 'Color-swapping puzzle platformer', image: dummy, blocks: painter },
          { slug: 'sonar-soul', title: 'Sonar Soul', subtitle: 'Sound-wave note-matching action game', image: dummy, blocks: sonarSoul },
          { slug: 'dragonsword-legend', title: 'DragonSword Legend', subtitle: 'Console RPG written in C (2015)', image: dummy, blocks: dragonswordLegend },
        ],
      },
    ],
  },
}

export const toSectionPath = (category: CategoryKey, slug: string, child?: string) =>
  `/${category}/${slug}${child ? `/${child}` : ''}`

export function getProject(category: CategoryKey, slug?: string, child?: string) {
  const cat = CATEGORIES[category]
  const project = cat.projects.find((p) => p.slug === slug)
  if (!project) return null
  if (child && !project.children?.some((c) => c.slug === child)) return null
  return { category: cat, project }
}
