


////////////////////: Two-way communication between an iOS WKWebView add on
   var _selector = document.querySelector('#basis > .atome');
   _selector.addEventListener('change', function(event) {
       var message = (_selector.checked) ? "Toggle Switch is on" : "Toggle Switch is off";
       if (window.webkit && window.webkit.messageHandlers && window.webkit.messageHandlers.toggleMessageHandler) {
           window.webkit.messageHandlers.toggleMessageHandler.postMessage({
               "message": message
           });
       }
   });

