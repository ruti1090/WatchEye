WatchEye - Virtual Pool Lifeguard

A real-time drowning detection system designed to enhance pool safety through advanced computer vision and machine learning techniques.

Project Overview

WatchEye is an intelligent surveillance system that monitors swimming pools to identify early warning signs that may indicate potential drowning situations. By analyzing swimmer movements, postures, and behaviors in real-time, the system can detect concerning patterns and trigger immediate alerts to enable preventive intervention.

The system addresses the critical challenge of pool safety, where traditional lifeguarding may have limitations in continuously monitoring multiple swimmers simultaneously. WatchEye serves as a complementary safety layer, providing 24/7 vigilant monitoring.

Key Features

Real-Time Detection
- Continuous monitoring of pool areas using computer vision
- Multi-process algorithm for efficient real-time analysis
- Detection of abnormal swimming patterns and early warning signs

Behavioral Analysis
- Advanced movement pattern recognition
- Posture analysis to identify struggling swimmers

Intelligent Alert System
- Instant notifications when concerning behavioral patterns are detected

High Accuracy Detection
- YOLO v8 implementation for precise object detection
- PyTorch-based machine learning models
- Optimized for various lighting conditions and pool configurations

Technology Stack

Backend Development
- C++: Core processing engine for high-performance real-time analysis
- Multi-threading: Efficient parallel processing for simultaneous monitoring

Machine Learning & Computer Vision
- Python: Machine learning model development and integration
- PyTorch: Deep learning framework for drowning detection algorithms
- OpenCV: Computer vision library for image processing and analysis
- NVIDIA GPU Acceleration: Enhanced performance for real-time processing

 Object Detection
- YOLO v8: State-of-the-art object detection for movements swimmer 
- Custom trained models: Specialized for Certain postures and movements detection

System Architecture

The system operates through a multi-layered approach:

1. Video Input Processing: Captures live feed from pool surveillance cameras
2. Object Detection Layer: Identifies and tracks individuals in the water
3. Behavioral Analysis Engine: Analyzes movement patterns and postures
4. Risk Assessment Module: Evaluates concerning behavioral patterns and calculates risk levels based on detected indicators
5. Alert Management System: Triggers appropriate notifications and emergency protocols

Installation Requirements

System Prerequisites
- Operating System: Windows (required for Windows-specific libraries)
- Development Environment: Microsoft Visual Studio 2019 or higher
- Python Version: Python 3.10 
- C++ Standard: C++17 or higher
- Hardware: High-resolution camera system for pool monitoring

Development Environment Setup

Python Installation
1. Install Python 3.10 and ensure it's added to system PATH
2. Verify installation path matches the expected location:
      C:\Users\USER\AppData\Local\Programs\Python\Python310

Required Python Libraries
Install the following dependencies using pip:

```bash
# Core machine learning and computer vision
pip install torch torchvision torchaudio
pip install opencv-python
pip install ultralytics
pip install numpy
```

Visual Studio Configuration
To enable C++ and Python integration, configure the following in Visual Studio:

Include Directories:
```
C:\Users\USER\AppData\Local\Programs\Python\Python310\include
```

Library Directories:
```
C:\Users\USER\AppData\Local\Programs\Python\Python310\libs
```

Linker Dependencies:
- Add `python310.lib` to Additional Dependencies

Project Directory Structure

The system requires a specific directory structure to function properly. Ensure the following structure exists under `C:\Users\USER\Desktop`:

C:\Users\USER\Desktop\
└── PROJECT/
    ├── Threads/
    ├── cheakNewPerson/
    ├── firstrec/
    │   └── predict/
    │       └── labels/
    ├── NEWYOLO/
    │   └── best.pt
    ├── PicFromPool/
    └── yoloperson/
        ├── best.pt
        └── CMD.bat
```

Important Notes:

- YOLO model files (`best.pt`) must be placed in their respective directories
- All directory paths are hardcoded in the application
- Ensure proper file permissions for all directories

Compilation and Build Process

Building the Project
1. Open the solution file (`.sln`) in Visual Studio
2. Ensure all Python integration settings are configured correctly
3. Set the project to use 
C++17 standard or higher
4. Build the solution in Release mode for optimal performance

Troubleshooting Common Issues
- Python Integration Errors: Verify Python 3.10 installation path matches the hardcoded path in the code
- Missing Libraries: Ensure all Python dependencies are installed in the correct Python environment
- Directory Structure: Confirm all required directories exist with proper permissions
- Model Files: Verify that YOLO model files (`best.pt`) are present in their designated locations

Performance Metrics

The system has been tested and optimized for:
- Real-time processing: < 100ms response time
- Detection accuracy: High precision in identifying distress situations
- Multi-swimmer tracking: Simultaneous monitoring of multiple individuals

Safety Considerations

Important Notice: WatchEye is designed as a supplementary safety system and should not replace human lifeguards or proper swimming supervision. It serves as an additional layer of protection to enhance overall pool safety.

Future Enhancements

- Integration with emergency response systems
- Mobile app notifications for pool managers
- Historical data analysis for safety insights
- Support for multiple camera angles and pool configurations
- Edge device deployment for reduced latency

Development Timeline

2025 - Final Project Development
- Algorithm design and implementation
- Integration of machine learning models
- Real-time processing optimization
- Comprehensive testing and validation

Technical Challenges Addressed

- Real-time Processing: Optimized algorithms for immediate response
- Environmental Variability: Robust detection across different lighting and weather conditions
- Multi-target Tracking: Simultaneous monitoring of multiple swimmers
-Long-term monitoring of a specific person

Contributing

This project was developed as a final academic project focusing on practical applications of computer vision and machine learning in safety-critical systems.

License

This project is developed for educational and safety research purposes.
---
Technology serving life - every second counts in water safety

