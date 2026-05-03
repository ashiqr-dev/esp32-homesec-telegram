<a id="readme-top"></a>


<h1 align="center">esp32-homesec-telegram</h1>

  <p align="center">
    WIP Home security system
  </p>

<!-- ABOUT THE PROJECT -->
## About The Project

This is an ESP32-based home security system built using the ESP-IDF framework, currently a work-in-progress.
The goal is to gradually evolve this into a full-featured system utilizing a Telegram bot to receive commands and send output.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- PREREQUISITES -->
## Prerequisites

 - Git
 - Docker
 - VS Code
 - VS Code Dev Containers extension

### Install Docker

Follow the official Docker installation guide:

https://docs.docker.com/engine/install/

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- HOW TO USE -->
## How to Use

1. Clone the repository:
    ```sh
    git clone https://github.com/ashiqr-dev/esp32-homesec-telegram.git
    cd esp32-homesec-telegram
    ```
2. Open the project in the VS Code Dev Container (`F1` -> `Reopen in Container`).


> **Optional: IntelliSense**
>
> For VS Code IntelliSense to work, it needs its configuration files.
> This will **not** affect the building and flashing of the firmware.
> To generate the config files, run the following command:
>
> `F1` -> `ESP-IDF: Add VS Code Configuration Folder`

3. To edit Wi-Fi settings:
    ```sh
    idf.py menuconfig
    ```

4. Scroll to and open `Project configuration`.

5. Edit Wi-Fi configuration and save it.

6. To build and flash, run 
    ```sh
    idf.py build flash
    ```

7. To monitor serial output:
    ```sh
    idf.py monitor
    ```

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- LICENSE -->
## License

Distributed under the Apache 2.0 License. See `LICENSE` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTACT -->
## Contact

Ashiq Renju - [@ashiqr-dev](https://github.com/ashiqr-dev/) - ashiqr.dev@gmail.com

Project Link: [https://github.com/ashiqr-dev/esp32-homesec-telegram/](https://github.com/ashiqr-dev/esp32-homesec-telegram/)


<p align="right">(<a href="#readme-top">back to top</a>)</p>
