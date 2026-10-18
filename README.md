<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<a id="readme-top"></a>
<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Don't forget to give the project a star!
*** Thanks again! Now go create something AMAZING! :D
-->



<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![Apache 2.0][license-shield]][license-url]



<!-- PROJECT LOGO -->
<!-- <br /> -->
<div align="center">
  <!-- <a href="https://github.com/ashiqr-dev/esp32-homesec-telegram">
    <img src="images/logo.png" alt="Logo" width="80" height="80">
  </a> -->

<h3 align="center">esp32-homesec-telegram (wip)</h3>

  <!-- <p align="center">
    project_description
    <br />
    <a href="https://github.com/ashiqr-dev/esp32-homesec-telegram"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/ashiqr-dev/esp32-homesec-telegram">View Demo</a>
    ·
    <a href="https://github.com/ashiqr-dev/esp32-homesec-telegram/issues/new?labels=bug&template=bug-report---.md">Report Bug</a>
    ·
    <a href="https://github.com/ashiqr-dev/esp32-homesec-telegram/issues/new?labels=enhancement&template=feature-request---.md">Request Feature</a>
  </p> -->
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

<!-- [![Product Name Screen Shot][product-screenshot]](https://example.com) -->

esp32-homesec-telegram is an open-source home security project built using ESP-IDF and the ESP32. The goal of the project is to create a home security system that integrates with Telegram for remote commands and status updates.

The project is written in C and uses ESP-IDF's APIs. It is a work in progress, with plans to expand its configuration options.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- ### Built With

<p align="right">(<a href="#readme-top">back to top</a>)</p> -->



<!-- GETTING STARTED -->
## Getting Started

### Prerequisites

- ESP-IDF (or the VS code ESP-IDF extension)
- Activated ESP-IDF environment

### Installation

1. Clone the repository
   ```sh
   git clone https://github.com/ashiqr-dev/esp32-homesec-telegram.git
   cd esp32-homesec-telegram
   ```
2. Set up ESP-IDF

   Select the ESP32 target:
   ```sh
   idf.py set-target esp32
   ```
3. Build the project
   ```sh
   idf.py build
   ```
4. Flash the ESP32
   Connect your ESP32 board and run:
   ```sh
   idf.py flash
   ```
5. Monitor serial output (optional)
   ```sh
   idf.py monitor
   ```
   Or flash the firmware and start the serial monitor in one command:
    ```sh
    idf.py flash monitor
    ```

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Usage

Usage instructions will be updated as the project evolves.

<!-- _For more examples, please refer to the [Documentation](https://example.com)_ -->

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- ROADMAP -->
## Roadmap

- [ ] GPIO interrupts
- [ ] Telegram
<!-- - [ ] Feature 3
    - [ ] Nested Feature -->

See the [open issues](https://github.com/ashiqr-dev/esp32-homesec-telegram/issues) for a full list of proposed features (and known issues).

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTRIBUTING -->
<!-- ## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#readme-top">back to top</a>)</p> -->

<!-- ### Top contributors:

<a href="https://github.com/ashiqr-dev/esp32-homesec-telegram/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=ashiqr-dev/esp32-homesec-telegram" alt="contrib.rocks image" />
</a> -->



<!-- LICENSE -->
## License

Distributed under the Apache License 2.0. See `LICENSE` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTACT -->
## Contact

Ashiq Renju - ashiqr.dev@gmail.com

Project Link: [https://github.com/ashiqr-dev/esp32-homesec-telegram](https://github.com/ashiqr-dev/esp32-homesec-telegram)

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

* [Best-README-Template](https://github.com/othneildrew/Best-README-Template)
* [ESP-IDF](https://github.com/espressif/esp-idf)
<!-- * []() -->

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/ashiqr-dev/esp32-homesec-telegram.svg?style=for-the-badge
[contributors-url]: https://github.com/ashiqr-dev/esp32-homesec-telegram/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/ashiqr-dev/esp32-homesec-telegram.svg?style=for-the-badge
[forks-url]: https://github.com/ashiqr-dev/esp32-homesec-telegram/network/members
[stars-shield]: https://img.shields.io/github/stars/ashiqr-dev/esp32-homesec-telegram.svg?style=for-the-badge
[stars-url]: https://github.com/ashiqr-dev/esp32-homesec-telegram/stargazers
[issues-shield]: https://img.shields.io/github/issues/ashiqr-dev/esp32-homesec-telegram.svg?style=for-the-badge
[issues-url]: https://github.com/ashiqr-dev/esp32-homesec-telegram/issues
[license-shield]: https://img.shields.io/github/license/ashiqr-dev/esp32-homesec-telegram.svg?style=for-the-badge
[license-url]: https://github.com/ashiqr-dev/esp32-homesec-telegram/blob/master/LICENSE

[esp-idf-url]: https://github.com/espressif/esp-idf