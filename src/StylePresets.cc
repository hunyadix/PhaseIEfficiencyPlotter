#include "../interface/StylePresets.h"

namespace StylePresets
{
	void markersOnly(TH1* t_histogram, int t_markerStyle, int t_markerColor)
	{
		t_histogram -> SetMarkerStyle(t_markerStyle);
		t_histogram -> SetMarkerColor(t_markerColor);
	}
};