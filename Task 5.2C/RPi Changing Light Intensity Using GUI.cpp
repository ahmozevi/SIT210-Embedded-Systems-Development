#include <QApplication>      // For creating a Qt application
#include <QMainWindow>       // Main window class
#include <QSlider>           // Slider widget to control LED intensity
#include <QPushButton>       // Exit button to close the program
#include <QVBoxLayout>       // Layout to arrange the widgets
#include <QLabel>            // For labeling the sliders
#include <wiringPi.h>        // For controlling Raspberry Pi GPIO pins
#include <softPwm.h>         // For controlling the LEDs with software PWM

class LEDControlWindow : public QMainWindow {
    Q_OBJECT  // This is necessary for Qt's signal and slot mechanism

public:
    LEDControlWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        // Initialize WiringPi for GPIO control
        if (wiringPiSetupGpio() == -1) {
            exit(1);  // Exit if setup fails
        }

        // Set GPIO pins for software PWM control (use Physical pins 11, 13, 15)
        pinMode(RED_PIN, OUTPUT);     // Red LED on GPIO 17 (Physical pin 11)
        pinMode(GREEN_PIN, OUTPUT);   // Green LED on GPIO 27 (Physical pin 13)
        pinMode(BLUE_PIN, OUTPUT);    // Blue LED on GPIO 22 (Physical pin 15)

        // Initialize software PWM for all LEDs (range is 0-32)
        softPwmCreate(RED_PIN, 0, 32);   // Red LED (PWM range 0 to 32)
        softPwmCreate(GREEN_PIN, 0, 32); // Green LED
        softPwmCreate(BLUE_PIN, 0, 32);  // Blue LED

        setAllLEDs(0);  // Start with all LEDs turned off

        // Set up the layout and central widget for the GUI
        QWidget *centralWidget = new QWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(centralWidget);

        // Create sliders and labels for each LED
        QLabel *redLabel = new QLabel("Red LED Intensity ");
        redSlider = new QSlider(Qt::Horizontal);  // Horizontal slider for red LED
        redSlider->setRange(0, 32);  // Set slider range (0 to 32)
        redSlider->setValue(0);       // Start with red LED off

        QLabel *greenLabel = new QLabel("Green LED Intensity ");
        greenSlider = new QSlider(Qt::Horizontal);  // Horizontal slider for green LED
        greenSlider->setRange(0, 32);  // Set slider range (0 to 32)
        greenSlider->setValue(0);       // Start with green LED off

        QLabel *blueLabel = new QLabel("Blue LED Intensity ");
        blueSlider = new QSlider(Qt::Horizontal);  // Horizontal slider for blue LED
        blueSlider->setRange(0, 32);  // Set slider range (0 to 32)
        blueSlider->setValue(0);      // Start with blue LED off

        // Create Exit button to quit the program
        exitButton = new QPushButton("Exit Program");

        // Add sliders and button to the layout
        layout->addWidget(redLabel);
        layout->addWidget(redSlider);
        layout->addWidget(greenLabel);
        layout->addWidget(greenSlider);
        layout->addWidget(blueLabel);
        layout->addWidget(blueSlider);
        layout->addWidget(exitButton);

        // Set the layout of the central widget
        setCentralWidget(centralWidget);

        // Connect slider changes to functions that update LED brightness
        connect(redSlider, &QSlider::valueChanged, this, &LEDControlWindow::onRedChanged);
        connect(greenSlider, &QSlider::valueChanged, this, &LEDControlWindow::onGreenChanged);
        connect(blueSlider, &QSlider::valueChanged, this, &LEDControlWindow::onBlueChanged);
        connect(exitButton, &QPushButton::clicked, this, &LEDControlWindow::onExitClicked);

        // Set up the window title and size
        setWindowTitle("LED Intensity Control (Software PWM)");
        resize(400, 300);  // Set window size to 400x300 pixels
    }

    ~LEDControlWindow() {
        setAllLEDs(0);  // Turn off all LEDs when the window is closed
    }

private slots:
    // These slots are called when the slider values change
    void onRedChanged(int value) {
        softPwmWrite(RED_PIN, value);  // Set the PWM duty cycle for the red LED
    }

    void onGreenChanged(int value) {
        softPwmWrite(GREEN_PIN, value);  // Set the PWM duty cycle for the green LED
    }

    void onBlueChanged(int value) {
        softPwmWrite(BLUE_PIN, value);  // Set the PWM duty cycle for the blue LED
    }

    // Slot for the Exit button
    void onExitClicked() {
        setAllLEDs(0);  // Turn off all LEDs when the Exit button is clicked
        QApplication::quit();  // Quit the application
    }

private:
    // Sliders and button for the UI
    QSlider *redSlider;
    QSlider *greenSlider;
    QSlider *blueSlider;
    QPushButton *exitButton;

    // GPIO pin numbers (for Raspberry Pi)
    const int RED_PIN = 17;   // GPIO 17 (Physical pin 11)
    const int GREEN_PIN = 27; // GPIO 27 (Physical pin 13)
    const int BLUE_PIN = 22;  // GPIO 22 (Physical pin 15)

    // Helper function to set all LEDs to the same value (used for turning off LEDs)
    void setAllLEDs(int value) {
        softPwmWrite(RED_PIN, value);    // Set red LED PWM
        softPwmWrite(GREEN_PIN, value);  // Set green LED PWM
        softPwmWrite(BLUE_PIN, value);   // Set blue LED PWM
    }
};

// Main function to run the application
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);  // Initialize the Qt application
    LEDControlWindow window;       // Create the main window
    window.show();                 // Show the window
    return app.exec();             // Start the Qt event loop
}


#include "led_control_intensity.moc"