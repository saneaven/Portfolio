import numpy as np


class NeuralNetwork:
    def __init__(self, input_size, hidden_layers, output_size, activation_type='relu', last_layer_activation=None):
        self.input_size = input_size
        self.hidden_layers = hidden_layers
        self.output_size = output_size
        self.activation_type = activation_type
        self.last_layer_activation = last_layer_activation

        self._initialize_variables()
        
    def _initialize_variables(self):
        """Initialize weights and biases for the neural network using He initialization."""

        layer_sizes = [self.input_size] + self.hidden_layers + [self.output_size]
        self.variables = []
        for i in range(len(layer_sizes) - 1):
            # He initialization
            weight_matrix = np.random.normal(0, np.sqrt(2 / layer_sizes[i]), (layer_sizes[i], layer_sizes[i + 1]))
            # initialize biases to zero
            bias_vector = np.zeros((1, layer_sizes[i + 1]))
            self.variables.append((weight_matrix, bias_vector))

    def _relu(self, x):
        """ReLU activation function."""
        return np.maximum(0, x)
    
    def _relu_derivative(self, x):
        """Derivative of ReLU activation function."""
        return (x > 0).astype(float)
    
    def _sigmoid(self, x):
        """Sigmoid activation function."""
        return 1 / (1 + np.exp(-x))
    
    def _sigmoid_derivative(self, x):
        """Derivative of Sigmoid activation function."""
        sig = self._sigmoid(x)
        return sig * (1 - sig)
    
    def activation(self, X, activation_type: str | None = 'relu'):
        """Apply activation function."""

        match(activation_type):
            case 'relu':
                return self._relu(X)
            case 'sigmoid':
                return self._sigmoid(X)
            case None:
                return X
            case _:
                raise ValueError("Unsupported activation type. Use 'relu', 'sigmoid', or None.")
            
    def activiation_derivative(self, X, activation_type: str | None = 'relu'):
        """Apply derivative of activation function."""

        match(activation_type):
            case 'relu':
                return self._relu_derivative(X)
            case 'sigmoid':
                return self._sigmoid_derivative(X)
            case None:
                return np.ones_like(X)
            case _:
                raise ValueError("Unsupported activation type. Use 'relu', 'sigmoid', or None.")

    def predict(self, X: np.ndarray):
        """Predict output for given input data."""

        A = X.copy()
        for i, (weights, biases) in enumerate(self.variables):
            A = np.dot(A, weights) + biases

            if i != len(self.variables) - 1:
                A = self.activation(A, activation_type=self.activation_type)
            else:
                A = self.activation(A, activation_type=self.last_layer_activation)

        return A
    
    def _feed_forward(self, X: np.ndarray):
        """Perform a feedforward pass through the network."""
        A = X.copy()
        self.Z_values = [None]
        self.A_values = [X]
        for i, (weights, biases) in enumerate(self.variables):
            A = np.dot(A, weights) + biases
            self.Z_values.append(A)
            
            if i != len(self.variables) - 1:
                A = self.activation(A, activation_type=self.activation_type)
            else:
                A = self.activation(A, activation_type=self.last_layer_activation)
            
            self.A_values.append(A)
        
        return A
    
    def back_propagate(self, output, result, learning_rate=0.01):
        """Perform backpropagation and update weights and biases."""

        error = output - result # Assuming mean squared error loss derivative

        for i, (weights, biases) in reversed(list(enumerate(self.variables))):
            activation_type = self.last_layer_activation if i == len(self.variables) - 1 else self.activation_type
            deriv = self.activiation_derivative(self.Z_values[i + 1], activation_type=activation_type)
            output = self.A_values[i]

            W_delta = np.dot(output.T, error * deriv)
            b_delta = np.sum(error * deriv, axis=0, keepdims=True)

            weights -= learning_rate * W_delta
            biases -= learning_rate * b_delta

            error = np.dot(error * deriv, weights.T)
        
    def train(self, X: np.ndarray, y: np.ndarray, epochs=1000, learning_rate=0.01):
        """Train the neural network."""
        for epoch in range(epochs):
            if epoch % (epochs / 10) == 0:
                print(f"Epoch {epoch}")
                predictions = self.predict(X)
                loss = np.mean((predictions - y) ** 2)
                print(f"Loss: {loss}")

            output = self._feed_forward(X)
            self.back_propagate(output, y, learning_rate)
