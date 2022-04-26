//
//  AudioUnitViewController.swift
//  vieauv3
//
//  Created by jeezs on 23/04/2022.
//

import CoreAudioKit
import WebKit

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
       
    }

    
    public func createAudioUnit(with componentDescription: AudioComponentDescription) throws -> AUAudioUnit {
        audioUnit = try vieauv3AudioUnit(componentDescription: componentDescription, options: [])
        
        return audioUnit!
    }
    
}
