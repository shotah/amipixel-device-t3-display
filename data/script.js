function getParameterByName(name, url) {
    if (!url) url = window.location.href;
    name = name.replace(/[\[\]]/g, '\\$&');
    var regex = new RegExp('[?&]' + name + '(=([^&#]*)|&|#|$)'),
        results = regex.exec(url);
    if (!results) return null;
    if (!results[2]) return '';
    return decodeURIComponent(results[2].replace(/\+/g, ' '));
}

document.addEventListener('DOMContentLoaded', function () {
    const errorParam = getParameterByName('error');
    const successParam = getParameterByName('success');
    const ssidDropdown = document.getElementById('ssid');
    const scanButton = document.getElementById('scanButton');
    const errorDiv = document.getElementById('error');
    const successDiv = document.getElementById('success');

    const passwordInput = document.getElementById('password');
    const passwordToggle = document.getElementById('password-toggle');
    const eyeIcon = document.getElementById('eye-icon');

    passwordToggle.addEventListener('click', function () {
        try {
            if (passwordInput.type === 'password') {
                passwordInput.type = 'text';
                eyeIcon.style.backgroundImage = "url('img/eye-open.png?t=" + new Date().getTime() + "')";
            } else {
                passwordInput.type = 'password';
                eyeIcon.style.backgroundImage = "url('img/eye-closed.png?t=" + new Date().getTime() + "')";
            }
        } catch (error) {
            console.error('Error toggling password visibility:', error);
        }
    });

    function populateSSIDDropdown() {
        fetch('/ssids')
            .then(response => response.json())
            .then(ssids => {
                if (ssids.length > 0) { // Check if SSIDs are available
                    ssidDropdown.innerHTML = '<option value="" disabled selected>Select WiFi Network</option>';
                    ssids.forEach(ssid => {
                        const option = document.createElement('option');
                        option.value = ssid;
                        option.text = ssid;
                        ssidDropdown.appendChild(option);
                    });
                    scanButton.textContent = "Scan for Networks";
                } else {
                    setTimeout(populateSSIDDropdown, 500);
                }
            })
            .catch(error => {
                console.error('Error fetching WiFi networks:', error);
                errorDiv.textContent = 'Error fetching WiFi networks. Please try again or enter SSID manually.';
                errorDiv.style.display = 'block';
                scanButton.textContent = "Scan for Networks";
            });
    }

    function triggerScan() {
        scanButton.textContent = "Waiting for networks...";
        fetch('/scan-trigger')
            .then(response => {
                populateSSIDDropdown(); // Start polling after scan trigger
            })
            .catch(error => {
                console.error('Error triggering scan:', error);
                errorDiv.textContent = 'Error triggering WiFi scan. Please try again.';
                errorDiv.style.display = 'block';
                scanButton.textContent = "Scan for Networks";
            });
    }

    scanButton.addEventListener('click', triggerScan);
    populateSSIDDropdown();

    if (errorParam) {
        errorDiv.textContent = errorParam;
        errorDiv.style.display = 'block';
    }

    if (successParam) {
        successDiv.textContent = successParam;
        successDiv.style.display = 'block';
    }
});