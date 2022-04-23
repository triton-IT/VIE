//
//  AudioUnitViewController.swift
//  VieiOS
//
//  Created by C1i Pro on 24/02/2022.
//

import CoreAudioKit
import UIKit
import WebKit


public class AudioUnitViewController: AUViewController, AUAudioUnitFactory {
    var audioUnit: AUAudioUnit?

    @IBOutlet weak var webView: WKWebView!
    
    public override func viewDidLoad() {
        super.viewDidLoad()
        
        let myURL = URL(string:"https://www.apple.com/")
        let myRequest = URLRequest(url: myURL!)
        webView.load(myRequest)
        
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
        audioUnit = try VieiOSAudioUnit(componentDescription: componentDescription, options: [])

        return audioUnit!
    }

}
