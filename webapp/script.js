$(document).ready(function() {
    let isWindowOpen = false;
    let isFanOn = false;

    // Dark mode initialization
    const darkMode = localStorage.getItem('darkMode') === 'enabled';
    if (darkMode) {
        document.body.setAttribute('data-theme', 'dark');
        $('#darkModeCheckbox').prop('checked', true);
    }

    // Dark mode toggle handler
    $('#darkModeCheckbox').on('change', function() {
        if (this.checked) {
            document.body.setAttribute('data-theme', 'dark');
            localStorage.setItem('darkMode', 'enabled');
        } else {
            document.body.removeAttribute('data-theme');
            localStorage.setItem('darkMode', 'disabled');
        }
    });

    function updateSensorData() {
        $.ajax({
            url: '/data',
            method: 'GET',
            success: function(data) {
                // Update temperature
                $('#temperature').text(data.temperature.toFixed(1));
                
                // Update humidity
                $('#humidity').text(data.humidity.toFixed(1));
                
                // Update soil moisture
                $('#moisture').text(data.moisture.toFixed(1));
                
                // Update light level
                $('#light').text(data.light.toFixed(1));
                
                // Update pump status
                $('#pump').text(data.pump ? 'Running' : 'Off')
                    .removeClass('online offline')
                    .addClass(data.pump ? 'online' : 'offline');
                
                // Update grow lights status
                $('#growlights').text(data.growlights ? 'On' : 'Off')
                    .removeClass('online offline')
                    .addClass(data.growlights ? 'online' : 'offline');

                // Update window status
                isWindowOpen = data.window;
                $('#window').text(data.window ? 'Open' : 'Closed')
                    .removeClass('online offline')
                    .addClass(data.window ? 'online' : 'offline');

                // Update fan status
                isFanOn = data.fan;
                $('#fan').text(data.fan ? 'Running' : 'Off')
                    .removeClass('online offline')
                    .addClass(data.fan ? 'online' : 'offline');
                
                // Update fan emoji animation
                $('.fan-emoji').toggleClass('fan-spinning', data.fan);
            },
            error: function() {
                $('.sensor-value').text('--');
                console.log('Error fetching sensor data');
            }
        });
    }

    function updateWateringLog() {
        $.ajax({
            url: '/wateringlog',
            method: 'GET',
            success: function(data) {
                const tbody = $('#watering-log');
                tbody.empty();

                data.logs.forEach(log => {
                    const date = new Date(log.timestamp * 1000);
                    const formattedDate = date.toLocaleString();
                    
                    tbody.append(`
                        <tr>
                            <td>${formattedDate}</td>
                            <td>${log.moisture.toFixed(1)}%</td>
                        </tr>
                    `);
                });
            },
            error: function() {
                console.log('Error fetching watering log');
            }
        });
    }

    // Window control
    window.toggleWindow = function() {
        $.ajax({
            url: '/window',
            method: 'POST',
            data: { open: !isWindowOpen },
            success: function() {
                updateSensorData();
            },
            error: function() {
                console.log('Error controlling window');
            }
        });
    };

    // Fan control
    window.toggleFan = function() {
        $.ajax({
            url: '/fan',
            method: 'POST',
            data: { on: !isFanOn },
            success: function() {
                updateSensorData();
            },
            error: function() {
                console.log('Error controlling fan');
            }
        });
    };

    // Update sensor data every 5 seconds
    setInterval(updateSensorData, 5000);
    
    // Update watering log every 30 seconds
    setInterval(updateWateringLog, 30000);
    
    // Initial updates
    updateSensorData();
    updateWateringLog();
});
