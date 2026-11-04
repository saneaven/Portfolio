# Deep Learning

*Three studies that climb the deep-learning ladder: a neural net built from scratch in NumPy, convolutional networks ending in transfer learning, and a Double DQN agent.*

## Overview

This is a coursework collection of three deep-learning studies, deliberately ordered to move from first principles up to modern reinforcement learning. The first study builds a feedforward network and its backpropagation entirely by hand in NumPy, so the mechanics are never a black box. The second moves onto TensorFlow / Keras for the things that are impractical from scratch (convolutions, augmentation, pretrained backbones) across three image datasets. The third pushes the same tooling into decision-making, training a Double DQN agent to clean a grid world.

Each study is written to end on an honest finding rather than just a number: where ReLU dies, where extra layers stop helping, where a larger grid stops learning. The failures were where most of the understanding came from.

This work is solo: I implemented all three studies.

## The three studies

### Study 1: A feedforward network and backprop in pure NumPy

A small feedforward network (two inputs, two 16-unit hidden layers, one output) implemented entirely in NumPy in [`PrgHW1/nn_scratch.py`](PrgHW1/nn_scratch.py). It uses He initialisation, ReLU and Sigmoid activations and their derivatives, and full backpropagation that updates every weight and bias by hand. The task is to learn the product function `f(x, y) = x · y` from synthetic training triplets sampled in `[0, 1]`.

ReLU converged faster than Sigmoid and reached a lower RMSE against the random-guess baseline; Sigmoid was both slower over 20k epochs and less accurate. Forcing ReLU onto the output layer reproduced the classic "dying ReLU" (the loss flatlining instead of improving), a textbook failure made concrete by having built every step.

### Study 2: CNNs and transfer learning across three datasets

A move to TensorFlow / Keras, working through three progressively harder image datasets:

- **CIFAR-10, custom CNN.** A from-scratch convolutional stack (32 -> 64 -> 128 feature maps with max-pooling, then a dense head) trained with Adam. Validation accuracy landed well above the 10-class chance level, honestly assessed as "not high".
- **CIFAR-100, deeper and augmented.** A deeper convolutional stack (32 -> 64 -> 128 -> 256 -> 512 -> 1024 feature maps with max-pooling, dropout and a two-layer dense head) fronted by random flip / rotation / zoom / contrast augmentation, trained with Adam, tackling the 100-class "fine" split. A heavier variant that adds BatchNormalization and a cosine-decay SGD schedule with Nesterov momentum lives in the submission notebook (see *Running the studies*).
- **Animal-10, transfer learning.** Swapping in a pretrained ResNet50V2 backbone (frozen, ImageNet weights) feeding a global-average-pool and a small dense head took accuracy from a from-scratch baseline up to roughly 95% on the ten-animal dataset, the headline lesson in standing on pretrained features.
- **Head-design ablation.** Comparing classifier heads on the ResNet backbone showed the simplest one wins: a global-average-pool plus a small dense head beat heavier variants, where extra convolutional and dense layers only added overfitting or interference.

The dataset was split 8:1:1 (train / validation / test) via Keras' `image_dataset_from_directory`, and the class imbalance was left intentionally unbalanced as part of the experiment.

### Study 3: A Double DQN agent that cleans a grid

A custom gymnasium-style environment places `p` dirty tiles on an `m × n` grid. The agent starts at a corner and sees a multi-channel image state (one channel for its own position, one for the dirt map) and moves in four directions (N, S, E, W). The reward function rewards cleaning a tile and completing the board, and penalises each step and any move into a wall.

A Double DQN learns the policy: separate online and target convolutional networks, where the online net selects the next action and the target net evaluates it. It uses a 500k-capacity replay buffer, ε-greedy exploration decaying from 1.0 toward 0.01, soft target updates at τ = 0.005, Huber loss and gradient clipping (Adam with `clipnorm`). Training scales from a 5×5 board up to 15×15. On fixed dirt positions the agent learned a near-optimal path; randomising the dirt each episode destabilised learning. That is a clear lesson in how much the problem setup decides whether an agent learns at all. A Pygame visualisation renders the environment.

## Architecture

Each study has its own model structure, grounded in the source:

| Study | Structure |
| --- | --- |
| From scratch | Input (2) -> Dense 16 (ReLU) -> Dense 16 (ReLU) -> Output (1); He init, hand-written backprop |
| CNN / transfer | Conv 32 -> 64 -> 128 (3×3, max-pool) -> Dense -> softmax; then a frozen ResNet50V2 backbone -> GAP -> dense head |
| Double DQN | Grid state (rows × cols × 2) -> Conv 64 -> 128 -> 256 (3×3) -> Dense 256 -> Dense 128 -> Q-values (Dense 4, linear) |

## Tech stack

- **From scratch:** Python, NumPy (manual backpropagation, He initialisation, ReLU / Sigmoid).
- **Frameworks:** TensorFlow / Keras (convolutional networks, data augmentation, BatchNormalization, cosine-decay SGD, pretrained ResNet50V2 transfer learning).
- **Reinforcement learning:** Double DQN with online + target networks, a replay buffer, ε-greedy exploration, soft target updates and Huber loss.
- **Tooling:** Pygame (RL environment rendering), Matplotlib (training curves), a gymnasium-style environment.

## Running the studies

Each study lives in its own folder and was developed in Jupyter / a Python environment with NumPy, TensorFlow / Keras, Matplotlib and (for Study 3) Pygame.

- **Study 1:** `PrgHW1/`. The network is a plain module, [`nn_scratch.py`](PrgHW1/nn_scratch.py); the notebook (`Prg1 Assignment1.ipynb`) generates the synthetic data, trains, and plots RMSE over epochs.
- **Study 2:** `PrgHW2/`. Open `Prg2.ipynb` and run top to bottom. CIFAR-10 and CIFAR-100 load directly from Keras; the Animal-10 part expects the image set under `PrgHW2/animal10/raw-img/<class>/`. Here the CIFAR-100 cell trains the augmented Adam CNN described above; the heavier BatchNormalization + cosine-decay SGD/Nesterov CIFAR-100 variant is in the submission notebook, `Minjae_Kyung_Prg2.ipynb`.
- **Study 3:** `prgHW3/`. Open `prg3.ipynb` to define the environment and agent and train each grid size. Trained models are saved as `dqn_<size>.keras` and training curves as PNGs.

## Project structure

```
DL-Learning/
├── PrgHW1/                       # Study 1: net from scratch
│   ├── nn_scratch.py             # NeuralNetwork: He init, ReLU/Sigmoid, manual backprop
│   ├── Prg1 Assignment1.ipynb    # data generation, training, RMSE plots
│   └── Prg 1 Deep Learning.pdf   # assignment write-up
├── PrgHW2/                       # Study 2: CNNs & transfer learning
│   ├── Prg2.ipynb                # CIFAR-10, CIFAR-100 (Adam), Animal-10 / ResNet50V2
│   ├── Minjae_Kyung_Prg2.ipynb  # submission notebook: BatchNorm + cosine-decay SGD/Nesterov CIFAR-100 variant
│   ├── animal10/                 # Animal-10 image set + label translation
│   └── Prg 2 Deep Learning.pdf
└── prgHW3/                       # Study 3: deep reinforcement learning
    ├── prg3.ipynb                # VacuumEnv, ReplayBuffer, Double DQN agent, training
    ├── dqn_5x5.keras / 10x10 / 15x15
    ├── training_curves(*).png
    └── Prg 3 Deep Reinforcement Learning.pdf
```

## Links

- [Source on GitHub](https://github.com/saneaven/Portfolio/tree/main/projects/ai/DL-Learning): code and technical docs.

## Context

Solo deep-learning coursework at DigiPen, implementing all three studies: the from-scratch network, the CNN / transfer-learning bundle and the Double DQN agent. </content> </invoke>
