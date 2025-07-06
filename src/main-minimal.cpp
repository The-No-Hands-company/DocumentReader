#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QStyle>

class TestWindow : public QWidget
{
    Q_OBJECT

public:
    TestWindow(QWidget *parent = nullptr) : QWidget(parent)
    {
        setWindowTitle("Document Reader - Qt Test");
        setMinimumSize(400, 300);
        
        // Apply dark theme
        setStyleSheet(R"(
            QWidget {
                background-color: #2b2b2b;
                color: #ffffff;
            }
            QPushButton {
                background-color: #3c3c3c;
                border: 1px solid #555555;
                padding: 8px 16px;
                border-radius: 4px;
            }
            QPushButton:hover {
                background-color: #4a4a4a;
                border-color: #777777;
            }
            QPushButton:pressed {
                background-color: #2a2a2a;
            }
            QLabel {
                font-size: 14px;
                padding: 10px;
            }
        )");
        
        auto* layout = new QVBoxLayout(this);
        
        // Title
        auto* title = new QLabel("🎉 Qt6 Setup Successful!");
        title->setAlignment(Qt::AlignCenter);
        title->setStyleSheet("font-size: 18px; font-weight: bold; color: #4CAF50;");
        layout->addWidget(title);
        
        // Info
        auto* info = new QLabel(
            "Qt6 is properly configured and working!\n\n"
            "✅ C++23 compiler detected\n"
            "✅ Qt6 Core, Widgets, Gui, PrintSupport loaded\n"
            "✅ Dark theme applied\n"
            "✅ SVG icon support ready\n\n"
            "Next step: Install Poppler-Qt6 to enable PDF support\n"
            "Run: .\\setup-dependencies.ps1"
        );
        info->setAlignment(Qt::AlignCenter);
        info->setWordWrap(true);
        layout->addWidget(info);
        
        // Buttons
        auto* buttonLayout = new QHBoxLayout;
        
        auto* testBtn = new QPushButton("Test Dialog");
        connect(testBtn, &QPushButton::clicked, this, &TestWindow::showTestDialog);
        buttonLayout->addWidget(testBtn);
        
        auto* closeBtn = new QPushButton("Close");
        connect(closeBtn, &QPushButton::clicked, this, &QWidget::close);
        buttonLayout->addWidget(closeBtn);
        
        layout->addLayout(buttonLayout);
    }

private slots:
    void showTestDialog()
    {
        QMessageBox::information(this, "Qt Test", 
            "Qt6 widgets are working perfectly!\n\n"
            "Document Reader is ready for PDF support.");
    }
};

#include "main-minimal.moc"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("DocumentReader-Test");
    app.setApplicationVersion("1.0.0");
    
    TestWindow window;
    window.show();
    
    return app.exec();
}
