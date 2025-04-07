#include <QApplication>      // This lets us create a Qt application
#include <QMainWindow>       // This is the main window class
#include <QRadioButton>      // For selecting which LED to turn on
#include <QPushButton>       // For the Exit and OFF buttons
#include <QVBoxLayout>       // Puts the buttons in a vertical stack
#include <wiringPi.h>        // Used to control Raspberry Pi GPIO pins

// We create a custom window by inheriting from QMainWindow
class LEDControlWindow : public QMainWindow {
    Q_OBJECT  // Required for Qt's signals and slots to work

public:
    LEDControlWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        // Set up GPIO using BCM numbering (GPIO 17, 27, 22)
        wiringPiSetupGpio();

        // Set the GPIO pins as output
        pinMode(RED_PIN, OUTPUT);
        pinMode(GREEN_PIN, OUTPUT);
        pinMode(BLUE_PIN, OUTPUT);

        // Start with all LEDs turned off
        turnOffAllLEDs();

        // Create a widget to hold everything
        QWidget *centralWidget = new QWidget(this);

        // Create a layout to stack buttons vertically
        QVBoxLayout *layout = new QVBoxLayout(centralWidget);

        // Create buttons
        redButton = new QRadioButton("Turn ON Red LED (Pin 11)");
        greenButton = new QRadioButton("Turn ON Green LED (Pin 13)");
        blueButton = new QRadioButton("Turn ON Blue LED (Pin 15)");
        offButton = new QPushButton("Turn OFF All LEDs");
        exitButton = new QPushButton("Exit Program");

        // Add buttons to the layout
        layout->addWidget(redButton);
        layout->addWidget(greenButton);
        layout->addWidget(blueButton);
        layout->addWidget(offButton);
        layout->addWidget(exitButton);

        // Tell the window to use this layout
        setCentralWidget(centralWidget);

        // Connect button clicks to functions (called "slots")
        connect(redButton, &QRadioButton::clicked, this, &LEDControlWindow::onRedClicked);
        connect(greenButton, &QRadioButton::clicked, this, &LEDControlWindow::onGreenClicked);
        connect(blueButton, &QRadioButton::clicked, this, &LEDControlWindow::onBlueClicked);
        connect(offButton, &QPushButton::clicked, this, &LEDControlWindow::turnOffAllLEDs);
        connect(exitButton, &QPushButton::clicked, this, &LEDControlWindow::onExitClicked);

        // Set the title of the window and its size
        setWindowTitle("Raspberry Pi LED Control");
        resize(300, 250);
    }

    ~LEDControlWindow() {
        // When the window closes, make sure LEDs are turned off
        turnOffAllLEDs();
    }

private slots:
    // When each radio button is clicked, turn on the correct LED

    void onRedClicked() {
        turnOffAllLEDs();                  // Turn off others first
        digitalWrite(RED_PIN, HIGH);       // Turn on red
    }

    void onGreenClicked() {
        turnOffAllLEDs();
        digitalWrite(GREEN_PIN, HIGH);     // Turn on green
    }

    void onBlueClicked() {
        turnOffAllLEDs();
        digitalWrite(BLUE_PIN, HIGH);      // Turn on blue
    }

    void onExitClicked() {
        turnOffAllLEDs();   // Make sure all LEDs are off before exiting
        QApplication::quit();  // Quit the app
    }

private:
    // These are the buttons we'll see in the window
    QRadioButton *redButton;
    QRadioButton *greenButton;
    QRadioButton *blueButton;
    QPushButton *offButton;
    QPushButton *exitButton;

    // GPIO pin numbers (BCM numbers, not physical pin numbers)
    const int RED_PIN = 17;   // Physical pin 11
    const int GREEN_PIN = 27; // Physical pin 13
    const int BLUE_PIN = 22;  // Physical pin 15

    // Turn off all the LEDs (helper function)
    void turnOffAllLEDs() {
        digitalWrite(RED_PIN, LOW);
        digitalWrite(GREEN_PIN, LOW);
        digitalWrite(BLUE_PIN, LOW);
    }
};

// The main function - the program starts here
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);   // Create the app
    LEDControlWindow window;        // Make the main window
    window.show();                  // Show it on screen
    return app.exec();              // Start the Qt event loop
}

// This line is needed for Qt to make signals and slots work in one file
#include "led_control.moc"
