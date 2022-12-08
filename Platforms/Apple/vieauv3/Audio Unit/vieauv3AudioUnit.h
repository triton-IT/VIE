//
//  vieauv3AudioUnit.h
//  vieauv3
//
//  Created by jeezs on 23/04/2022.
//

#import <AudioToolbox/AudioToolbox.h>
#import "vieauv3DSPKernelAdapter.h"

// Define parameter addresses.
extern const AudioUnitParameterID myParam1;

@interface vieauv3AudioUnit : AUAudioUnit

@property (nonatomic, readonly) vieauv3DSPKernelAdapter *kernelAdapter;
- (void)setupAudioBuses;
- (void)setupParameterTree;
- (void)setupParameterCallbacks;
@end
