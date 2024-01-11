//
//  AudioUnitViewController.swift
//  vieauv3
//
//  Created by jeezs on 23/04/2022.
//

import CoreAudioKit
import WebKit
///////////////////////: Two-way communication between an iOS WKWebView add on
 extension AudioUnitViewController: WKScriptMessageHandler{
     
     
     public func userContentController(_ userContentController: WKUserContentController, didReceive message: WKScriptMessage) {
        guard let dict = message.body as? [String : AnyObject] else {
            return
        }

         guard let message = dict["message"] else {
             return
         }

         let script = "document.getElementById('value').innerText = \"\(message)\""
// here is the response meessage to the view
         webview.evaluateJavaScript(script) { (result, error) in
             if let result = result {
                 print("Label is updated with message: \(result)")
             } else if let error = error {
                 print("An error occurred: \(error)")
             }
         }
         
         
    }
}

public class AudioUnitViewController: AUViewController, AUAudioUnitFactory {
    
    
    
    var audioUnit: AUAudioUnit?
    
    @IBOutlet weak var webview: WKWebView!
    public override func viewDidLoad() {
        
        super.viewDidLoad()
        
        /// this one work
//        let myURL = URL(string:"https://www.apple.com/")
//        let myRequest = URLRequest(url: myURL!)
//        webview.load(myRequest)
        
        
        // this one may work
        let myProjectBundle:Bundle = Bundle.main
        let myUrl = myProjectBundle.url(forResource: "view/index", withExtension: "html")!
        webview.loadFileURL(myUrl,allowingReadAccessTo: myUrl)

        

    
        /// this one is very uncertain
//        if audioUnit == nil {
//            return
//        }
//
//        if let indexURL = Bundle.main.url(forResource: "index",
//                                          withExtension: "html",
//                                          subdirectory: "view"
         
//
//        ) {
//            self.webView.loadFileURL(indexURL,
//                                     allowingReadAccessTo: indexURL)
//        }
        
        ///////////////////////: Two-way communication between an iOS WKWebView add on
        let contentController = self.webview.configuration.userContentController
        contentController.add(self, name: "toggleMessageHandler")
        
        let js = """
        document.body.innerHTML += '<div style="position:absolute;width:100%;height:100%;opacity:0.3;z-index:100;background:#000;">hi from js injection from swift!</div>';
        //// test messaging
            var _selector = document.querySelector('input[name=myCheckbox]');
            _selector.addEventListener('change', function(event) {
                var message = (_selector.checked) ? "Toggle Switch is on" : "Toggle Switch is off";
                if (window.webkit && window.webkit.messageHandlers && window.webkit.messageHandlers.toggleMessageHandler) {
                    window.webkit.messageHandlers.toggleMessageHandler.postMessage({
                        "message": message
                    });
                }
            });
        

        """

        let script = WKUserScript(source: js, injectionTime: .atDocumentEnd, forMainFrameOnly: false)
        contentController.addUserScript(script)
        
        
        

       
    }

    
    public func createAudioUnit(with componentDescription: AudioComponentDescription) throws -> AUAudioUnit {
        audioUnit = try vieauv3AudioUnit(componentDescription: componentDescription, options: [])
        
        return audioUnit!
    }
    
}

