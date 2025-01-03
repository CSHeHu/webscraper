# WebScraper

A C++ desktop application for fetching and displaying web headlines dynamically. This project is built using **Qt** for the GUI and **libcurl** for HTTP requests. It offers a clean, scrollable interface and modular architecture for data management.

---

## 🚀 Features

<ul>
  <li>Fetches and displays headlines from web sources.</li>
  <li>Uses <b>Qt's Widgets module</b> for a responsive GUI.</li>
  <li>Integrates <b>libcurl</b> for seamless web data retrieval.</li>
  <li>Scroll area for dynamically loaded content.</li>
</ul>

---

## 🛠️ Dependencies

<ul>
  <li><b>Qt 6.8.1</b>: GUI development framework.</li>
  <li><b>libcurl</b>: Library for making HTTP requests.</li>
  <li><b>CMake 3.25+</b>: Build system generator.</li>
</ul>

<p>The project is written in <b>C++23</b>, so a compiler supporting C++23 is required.</p>

---

## 📦 Project Structure

<pre>
webscraper/
├── CMakeLists.txt         # Build configuration
├── src/                   # Source files (.cpp)
└── header/                # Header files (.h)
</pre>

---

## ⚙️ Building the Project

<p>Follow these steps to build and run the application:</p>

<ol>
  <li>Clone the repository:</li>
  <pre><code>git clone https://github.com/yourusername/webscraper.git</code></pre>
  <li>Navigate to the project directory:</li>
  <pre><code>cd webscraper</code></pre>
  <li>Configure the project with CMake:</li>
  <pre><code>cmake -S . -B build -DCMAKE_PREFIX_PATH=~/Qt/6.8.1/gcc_64</code></pre>
  <li>Build the project:</li>
  <pre><code>cmake --build build</code></pre>
  <li>Run the application:</li>
  <pre><code>./build/webscraper</code></pre>
</ol>

---

## 🖱️ Contributing

<p>Contributions are welcome! If you'd like to improve the project, please follow these steps:</p>

<ol>
  <li>Fork the repository.</li>
  <li>Create a new feature branch:</li>
  <pre><code>git checkout -b feature-branch</code></pre>
  <li>Commit your changes:</li>
  <pre><code>git commit -m "Add new feature"</code></pre>
  <li>Push the branch and create a pull request.</li>
</ol>

---

## 📜 License

<p>This project is licensed under the <a href="LICENSE">MIT License</a>. Feel free to use, modify, and distribute the software as per the terms of the license.</p>

---

## 🔗 References

<ul>
  <li><a href="https://doc.qt.io/">Qt Documentation</a></li>
  <li><a href="https://curl.se/libcurl/">libcurl Documentation</a></li>
  <li><a href="https://cmake.org/">CMake Documentation</a></li>
</ul>

