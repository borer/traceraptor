/*
 * Color.h
 *
 *  Created on: May 14, 2017
 *      Author: bogdan
 */

#ifndef SRC_COLOR_H_
#define SRC_COLOR_H_

#include <Vec.h>

namespace traceraptor {

template <size_t numSpectrumSamples>
class Color : public Vec<float, numSpectrumSamples> {
public:
    constexpr Color() {}
    
    constexpr Color(std::initializer_list<float> vv) {
        assert(numSpectrumSamples == vv.size());
        auto i = 0;
        for (auto&& e : vv) (*this)[i++] = e;
    }
    
    constexpr Color(const Vec<float, numSpectrumSamples>& vv) : Vec<float, numSpectrumSamples>{vv} {}

	Color CompressGamma() {
        Color<numSpectrumSamples> compressedColor;
        Vec<float, numSpectrumSamples> clampedColor = component_clamp((*this), 0.f, 1.f);
		for (size_t i = 0; i < numSpectrumSamples; i++) compressedColor[i] = std::sqrt(clampedColor[i]);
        return compressedColor;
	}
    
    float GetIntensity() {
        float intensity = (*this)[0] * 0.299f + (*this)[1] * 0.587f + (*this)[2] * 0.114f;
        return intensity;
    }
};
    
typedef Color<3> RGBColor;

} /* namespace traceraptor */

#endif /* SRC_COLOR_H_ */
