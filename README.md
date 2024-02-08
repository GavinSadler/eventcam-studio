# eventcam-studio

eventcam-studio is a program which will interface with the [SilkyEvCam EvC3A](https://www.centuryarks.com/images/product/sensor/silkyevcam/SilkyEvCam-USB_Spec_Rev102.pdf) event-based camera and [Basler Dart daA3840-45uc](https://docs.baslerweb.com/daa3840-45uc) frame-based camera.

## How to clone this repository

If you're reading this message, you have access to this repository! Yipeee! 🎉

Try cloning this repository by opening a command prompt window with the following command:

`$ git clone https://github.com/GavinSadler/eventcam-studio.git`

If you got some sort of error saying `git is an unrecognized command`, you probably need to install git. Navigate to [this website](https://gitforwindows.org/)
and install git onto your computer.

You may not simply be able to `git clone` this repository onto your machine. One solution is to create a set of SSH keys on
your machine and add them to your Github account. Github already has a few guides on this topic, which you can read in the following articles:

[Check to see if you have existing SSH keys](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/checking-for-existing-ssh-keys)\
[Generate new SSH keys](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent)\
[Add SSH keys to your Github account](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/adding-a-new-ssh-key-to-your-github-account)

After doing this, try cloning the repository again.

## Required software

In order to build this software, you will need several items:
- A C++ compiler
- An IDE
- Metavision's SDK
- Bassler's SDK
- Event-camera driver

### C++ compiler and IDE

Visual Studio (Not Visual Studio Code) is the preferred IDE for C++ development. If you would rather use VSCode, go right ahead, but you will still need to
install a C++ compiler on your system. Luckily, we can install both of these with the Visual Studio Installer. Download it here:

[You can download both here](https://visualstudio.microsoft.com/vs/features/cplusplus/)

When running the Visual Studio Installer, make sure that you select `Desktop development with C++` as this package specifically installs the C++ compiler.

### Metavision SDK, event-camera driver, and Baslar's Pylon SDK

Both SDKs and the event-camera driver can be downloaded and installed from their respective websites:

[Metavision SDK (Make sure to download version 4.4.0 or greater!)](https://files.prophesee.ai/share/dists/public/windows/baiTh5si/)\
[SilkyEvCam EvC3A driver (This is required to interface with our specific camera)](https://centuryarks.com/en/download/)\
[Basler's Pylon SDK (Download the latest version of the software suite)](https://www2.baslerweb.com/en/downloads/software-downloads/)

[Metavision Trigger info](https://docs.prophesee.ai/stable/hw/manuals/timing_interfaces.html?highlight=triggers#trigger-in)
