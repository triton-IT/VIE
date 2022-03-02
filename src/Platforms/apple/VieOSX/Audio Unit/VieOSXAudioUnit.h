//
//  VieOSXAudioUnit.h
//  VieOSX
//
//  Created by C1i Pro on 24/02/2022.
//

#import <AudioToolbox/AudioToolbox.h>
#import "VieOSXDSPKernelAdapter.h"

// Define parameter addresses.
extern const AudioUnitParameterID myParam1;

@interface VieOSXAudioUnit : AUAudioUnit

@property (nonatomic, readonly) VieOSXDSPKernelAdapter *kernelAdapter;
- (void)setupAudioBuses;
- (void)setupParameterTree;
- (void)setupParameterCallbacks;
@end
