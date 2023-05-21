let sidebarLinks = document.querySelectorAll(".sidebar-link");
let mainContent = document.querySelector(".main-content .text");
let sidebar = document.querySelector(".sidebar");
let closeBtn = document.querySelector("#btn");
let searchBtn = document.querySelector(".bx-search");

sidebarLinks.forEach(link => {
    link.addEventListener("click", (event) => {
        event.preventDefault();
        let file = link.dataset.file;
        let jsFile = link.dataset.js;
        loadContent(file, jsFile);
    });
});

function loadContent(htmlFile, jsFile) {
    // Fetch the HTML file
    fetch(htmlFile)
        .then(function (response) {
            return response.text();
        })
        .then(function (html) {
            // Get the main content element
            var mainContent = document.querySelector('.main-content');

            // Clear any existing content
            mainContent.innerHTML = '';

            // Set the HTML content
            mainContent.innerHTML = html;

            // If jsFile is provided, fetch and execute the JavaScript file
            if (jsFile) {
                fetch(jsFile)
                    .then(function (response) {
                        return response.text();
                    })
                    .then(function (js) {
                        // Execute the JavaScript code
                        eval(js);
                    })
                    .catch(function (error) {
                        console.error('Error loading JS file:', error);
                    });
            }
        })
        .catch(function (error) {
            console.error('Error loading HTML file:', error);
        });
}

closeBtn.addEventListener("click", () => {
    sidebar.classList.toggle("open");
    menuBtnChange();//calling the function(optional)
});

// Sidebar open when you click on the search iocn
searchBtn.addEventListener("click", () => {
    sidebar.classList.toggle("open");
    menuBtnChange(); //calling the function(optional)
});

// following are the code to change sidebar button(optional)
function menuBtnChange() {
    if (sidebar.classList.contains("open")) {
        closeBtn.classList.replace("bx-menu", "bx-menu-alt-right");//replacing the iocns class
    } else {
        closeBtn.classList.replace("bx-menu-alt-right", "bx-menu");//replacing the iocns class
    }
}

// Load dashboard content by default
loadContent('pages/dashboard.html');