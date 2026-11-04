import type { ContentBlocks } from '../contentblock/types'
import githubIcon from '../../../../assets/github-white-icon.svg'
import DeepLearningArchDiagram from './deep-learning.diagram'

const deepLearning: ContentBlocks = [
  {
    type: 'lead',
    eyebrow: 'AI · Deep Learning',
    title: 'Deep Learning',
    subtitle:
      'Three studies that climb the deep-learning ladder. The first implements a feedforward network and its backpropagation from scratch in pure NumPy. The second moves to convolutional networks across three image datasets, ending in transfer learning that jumps from ~68% to ~95% accuracy. The third trains a Double DQN agent to clean a grid world, scaling from a 5×5 board up to 15×15.',
    meta: [
      { label: 'Role', value: 'Solo — implemented all three studies' },
      { label: 'Focus', value: 'Backprop · CNNs · transfer learning · deep RL' },
      { label: 'Stack', value: 'NumPy · TensorFlow / Keras · Pygame' },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Overview',
    title: 'Understand the internals, then stand on the frameworks',
    blocks: [
      {
        type: 'text',
        paragraphs: [
          'The three studies are deliberately ordered. First, build a network with no framework at all — every weight update written by hand — so the mechanics are not a black box. Then switch to TensorFlow/Keras to do the things that are impractical from scratch: convolutions, augmentation, pretrained backbones. Finally, push the same tools into decision-making with reinforcement learning.',
          'Each study ends on an honest finding rather than just a number — where ReLU dies, where extra layers hurt, where a larger grid stops learning — because the failures were where most of the understanding came from.',
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Architecture',
    title: 'Three studies, three model structures',
    blocks: [
      {
        type: 'custom',
        render: () => <DeepLearningArchDiagram />,
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Study 1 — Nets from scratch',
    title: 'A feedforward network and backprop in pure NumPy',
    blocks: [
      {
        type: 'text',
        paragraphs: [
          'The first study implements a small feedforward network — two inputs, two 16-unit hidden layers, one output — entirely in NumPy, with He initialisation, ReLU and Sigmoid activations and their derivatives, and full backpropagation updating every weight and bias by hand. The task is to learn the product function f(x, y) = x · y from 100,000 synthetic samples.',
          'ReLU reached an RMSE of about 0.049 against a 0.25 baseline and converged faster than Sigmoid, which was both slower (43 vs 26 minutes over 20k epochs) and less accurate. Pushing ReLU onto the output layer reproduced the classic "dying ReLU" — loss flatlining around 0.334 — a textbook failure made concrete by having built every step.',
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Study 2 — CNNs & transfer learning',
    title: 'Three datasets, from scratch to pretrained',
    blocks: [
      {
        type: 'minis',
        items: [
          {
            title: 'CIFAR-10 — custom CNN',
            body: 'A from-scratch convolutional stack (32 → 64 → 128 feature maps with max-pooling, then dense layers) trained with Adam reached about 71.5% validation accuracy on the ten-class set — well above chance, honestly assessed as "not high".',
          },
          {
            title: 'CIFAR-100 — deeper & regularised',
            body: 'A deeper network with data augmentation, BatchNormalization and a cosine-decay SGD schedule with Nesterov momentum reached ~66.5% across one hundred fine-grained classes, meeting the target despite the harder split.',
          },
          {
            title: 'Animal-10 — transfer learning',
            body: 'Swapping in a pretrained ResNet50V2 backbone took accuracy from ~68% (trained from scratch) to ~95.5% on a 26k-image, ten-animal dataset — the headline lesson in standing on pretrained features.',
          },
          {
            title: 'Head-design ablation',
            body: 'Comparing four classifier heads on the ResNet backbone showed the simplest one wins: a global-average-pool plus a small dense head beat heavier variants, where extra layers and a sigmoid stage only added overfitting or interference.',
          },
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Study 3 — Deep reinforcement learning',
    title: 'A Double DQN agent that cleans a grid',
    blocks: [
      {
        type: 'text',
        paragraphs: [
          'A custom gymnasium-style environment places p dirty tiles on an m×n grid; the agent sees a multi-channel image state (its own position and the dirt map) and moves in four directions, rewarded for cleaning and completion and penalised per step and for hitting walls. A Double DQN — separate online and target convolutional networks — learns the policy with a 500k-capacity replay buffer, ε-greedy exploration decaying from 1.0 to 0.01, soft target updates at τ = 0.005, Huber loss and gradient clipping.',
          'It scales from a 5×5 board up to 15×15, where success on fixed dirt positions climbed to roughly 95% — while randomising the dirt each episode destabilised learning, a clear lesson in how much the problem setup decides whether an agent learns at all.',
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
          { layer: 'From scratch', items: ['NumPy', 'Manual backprop', 'He init', 'ReLU / Sigmoid'] },
          { layer: 'Frameworks', items: ['TensorFlow', 'Keras', 'ResNet50V2', 'Data augmentation'] },
          { layer: 'Reinforcement learning', items: ['Double DQN', 'Replay buffer', 'ε-greedy', 'Huber loss'] },
          { layer: 'Tooling', items: ['Pygame', 'gymnasium-style env', 'Python'] },
        ],
      },
    ],
  },
  {
    type: 'links',
    links: [
      {
        label: 'Source on GitHub',
        href: 'https://github.com/saneaven/Portfolio/tree/main/projects/ai/DL-Learning',
        icon: githubIcon,
        hint: 'Code & technical docs',
      },
    ],
  },
  {
    type: 'closing',
    text: 'Three solo studies up the deep-learning ladder — a network and backprop built by hand in NumPy, CNNs and transfer learning to ~95% accuracy, and a Double DQN agent cleaning a grid — each ending on what actually broke and why.',
  },
]

export default deepLearning
