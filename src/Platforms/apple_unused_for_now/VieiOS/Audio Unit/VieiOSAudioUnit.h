//
//  VieiOSAudioUnit.h
//  VieiOS
//
//  Created by C1i Pro on 24/02/2022.
//

#import <AudioToolbox/AudioToolbox.h>
#import "VieiOSDSPKernelAdapter.h"

// Define parameter addresses.
extern const AudioUnitParameterID myParam1;

@interface VieiOSAudioUnit : AUAudioUnit

@property (nonatomic, readonly) VieiOSDSPKernelAdapter *kernelAdapter;
- (void)setupAudioBuses;
- (void)setupParameterTree;
- (void)setupParameterCallbacks;
@end
