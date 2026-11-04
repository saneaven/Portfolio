import type { ContentBlocks } from '../contentblock/types'
import SpiderBotArchDiagram from './next-gen-dynamics.diagram'
import trainVideo from '../../../../assets/section/ai/nextgendynamics/train_video.webm'
import githubIcon from '../../../../assets/github-white-icon.svg'

const nextGenDynamics: ContentBlocks = [
  {
    type: 'lead',
    eyebrow: 'AI · Reinforcement Learning',
    title: 'Teaching a six-legged robot to walk, navigate and chase — learned in simulation.',
    subtitle:
      'Spider Bot AI trains a 24-DoF hexapod on NVIDIA\'s GPU physics simulator to cross procedurally generated rough terrain and carry out three behaviours — waypoint, patrol and chase — from one reinforcement-learning policy, then exports it to ONNX for sim-to-real deployment.',
    meta: [
      { label: 'Role', value: 'Solo redesign & reimplementation from a teammate\'s initial code' },
      { label: 'Focus', value: 'Deep RL — recurrent multi-modal PPO' },
      { label: 'Stack', value: 'Isaac Lab · skrl PPO_RNN · PyTorch · Docker' },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Overview',
    title: 'Learning to walk where rules can\'t be hand-coded',
    blocks: [
      {
        type: 'split',
        paragraphs: [
          'A many-legged robot has to decide which foot to place where, and when, every single moment — and the rougher the ground, the faster that problem explodes. Locomotion like this is hard to script by hand, so letting the robot learn to walk through reinforcement learning has become the standard approach.',
          'Spider Bot AI drops many hexapods at once onto procedurally generated terrain inside NVIDIA Isaac Lab (PhysX) and trains a walking policy with PPO. The result doesn\'t just walk: the same network routes to a destination, patrols unexplored ground, and chases an evading target, switching between modes on a signal.',
        ],
        media: { caption: 'Parallel hexapods training on procedural terrain', video: trainVideo },
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Core — RL Engineering',
    title: 'One recurrent policy that perceives, walks and plans',
    blocks: [
      {
        type: 'cardGrid',
        cards: [
          {
            title: 'Recurrent multi-modal policy',
            body: 'A GRU-based PPO_RNN built on skrl. Four independent encoders — proprioception (MLP), a 64×64 heightmap, a 3-channel BEV, and a 33×33 nav grid (ResBlock CNNs) — fuse to 320-D, pass through a GRU (hidden 512) over 32-step sequences, then a shared trunk branches into a Gaussian policy head and a value head.',
          },
          {
            title: 'One policy, three modes',
            body: 'Waypoint, patrol and chase are handled by a single network rather than three. A one-hot mode input scales 18+ reward terms — task rewards (progress, reach, exploration, pursuit) and motion-quality rewards (torque, smoothness, foot air-time, fall penalties) — so the same weights produce different behaviour.',
          },
          {
            title: 'Geodesic progress reward',
            body: 'Progress is measured by wall-aware geodesic distance (BFS over the nav grid), not straight-line distance, so the robot routes around obstacles instead of pushing into walls. Asymmetric log shaping around a ~1.5 m/s target speed suppresses overspeeding and reversing.',
          },
          {
            title: 'Procedural terrain & exploration',
            body: 'Perlin-noise hills, random cube/sphere obstacles and terraced zones are generated procedurally, sampled for flat spawn points, and converted to USD meshes — with SHA-256 config hashing for deterministic, reproducible caching. In patrol, a decaying staleness map drives the robot toward long-unvisited ground.',
          },
          {
            title: 'Sim-to-real via ONNX',
            body: 'On export, the running-standardisation statistics are baked into the model and the GRU hidden state is exposed as explicit I/O, so raw sensor input drives single-step recurrent inference with no external normalisation — built for edge deployment.',
          },
        ],
      },
      {
        type: 'custom',
        render: () => <SpiderBotArchDiagram />,
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Simulation & Environment',
    title: 'A robot, its senses and its world — built from scratch',
    blocks: [
      {
        type: 'minis',
        items: [
          {
            title: 'Manager-based RL environment',
            body: 'Built on Isaac Lab\'s ManagerBased architecture, with action, command, observation, reward, termination and event logic split into modules — so new reward terms, sensors or behaviour modes drop in without breaking the rest.',
          },
          {
            title: 'Custom 24-DoF hexapod',
            body: 'A self-built URDF: 6 legs × 4 joints, each modelled as an implicit PD actuator with its own range and torque limits. The network outputs target joint positions (offset from a default pose, scale 0.75) rather than raw torque, at 60 Hz control over 120 Hz physics.',
          },
          {
            title: 'Multi-modal sensing',
            body: 'Overlapping contact sensors on all 8 feet, a 64×64 raycast height scanner, a 32-channel 360° LiDAR and a frame transformer combine proprioception with a read on the surrounding terrain.',
          },
          {
            title: 'Reproducibility & containerisation',
            body: 'TensorBoard logging, timestamped run outputs, Omniverse debug markers and debug plots track training, all packaged in Docker Compose for both single-GPU and multi-GPU runs.',
          },
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Training',
    title: 'From parallel rollouts to a deployable policy',
    blocks: [
      {
        type: 'pipeline',
        title: 'PPO_RNN training loop',
        body: 'Many environments roll out in parallel on PhysX, GAE estimates advantages, and a clipped PPO objective updates the recurrent policy. KL-adaptive learning rate and running standardisation keep it stable, torch.distributed (DDP/NCCL) scales it across GPUs, and the trained policy exports to ONNX for sim-to-real.',
        steps: [
          'Rollout · 32 steps',
          'Standardize obs & reward',
          'GAE λ=0.95',
          'PPO update · clip 0.2',
          'KL-adaptive LR',
          'ONNX export → deploy',
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
          {
            layer: 'Simulation',
            items: ['Isaac Lab 3.0', 'Isaac Sim 6.0', 'PhysX (GPU)', 'Omniverse Kit'],
          },
          {
            layer: 'Reinforcement learning',
            items: ['skrl (custom PPO_RNN)', 'PyTorch', 'GRU policy', 'GAE', 'KL-adaptive LR', 'RunningStandardScaler'],
          },
          {
            layer: 'Perception & environment',
            items: ['Multi-modal obs', 'Contact · raycast · LiDAR', 'BFS geodesic nav', 'Procedural terrain'],
          },
          {
            layer: 'Robot',
            items: ['24-DoF hexapod URDF', 'Implicit PD actuators', 'Joint-position control'],
          },
          {
            layer: 'Deploy & infra',
            items: ['ONNX (sim-to-real)', 'torch.distributed DDP', 'Docker Compose', 'TensorBoard', 'Python 3.12'],
          },
        ],
      },
    ],
  },
  {
    type: 'links',
    links: [
      {
        label: 'Source on GitHub',
        href: 'https://github.com/saneaven/NextGenDynamics',
        icon: githubIcon,
        hint: 'Code & technical docs',
      },
    ],
  },
  {
    type: 'closing',
    text: 'Redesigned and rebuilt end to end — robot model, simulation, reward design, recurrent policy and the training-to-deployment pipeline.',
  },
]

export default nextGenDynamics
