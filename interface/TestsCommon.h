#ifndef TESTS_COMMON_H
#define TESTS_COMMON_H

#include <TH2D.h>

#include <memory>

void printUsage(int& argc, char** argv, bool killProcess, int argsType = 2)
{
	switch(argsType)
	{
		case 2:
			std::cout << "Usage: " << argv[0] << " <Ntuple path> <optional: --savePlots>" << std::endl;
			break;
		default:
			std::cout << "Undefined usage use-case." << std::endl;
	}
	if(killProcess) exit(-1);
}

void processArgs(int& argc, char** argv, std::string& inputFileName, int& savePlots)
{
	if(argc != 2 && argc != 3)
	{
		printUsage(argc, argv, true);
	}
	inputFileName = argv[1];
	if(argc == 3)
	{
		if(std::string(argv[2]) == std::string("--savePlots"))
		{
			savePlots = 1;
		}
		else
		{
			printUsage(argc, argv, true);
		}
	}
	else
	{
		savePlots = 0;
	}
}

void printClusterFieldInfo(const Cluster& clusterField)
{
	std::cout << "--- Begin cluster field info ---" << std::endl;
	std::cout << "x:          " << clusterField.x             << "." << std::endl;
	std::cout << "y:          " << clusterField.y             << "." << std::endl;
	std::cout << "clustSizeX: " << clusterField.sizeX         << "." << std::endl;
	std::cout << "clustSizeY: " << clusterField.sizeY         << "." << std::endl;
	std::cout << "clustIndex: " << clusterField.i  << "." << std::endl;
	std::cout << "charge:     " << clusterField.charge        << "." << std::endl;
	std::cout << "*** Associated module field info ***" << std::endl;
	std::cout << "det:        " << clusterField.mod_on.det    << "." << std::endl;
	std::cout << "layer:      " << clusterField.mod_on.layer  << "." << std::endl;
	std::cout << "module:     " << clusterField.mod_on.module << "." << std::endl;
	std::cout << "ladder:     " << clusterField.mod_on.ladder << "." << std::endl;
	std::cout << "--- End cluster field info ---" << std::endl;
}

void printTrajFieldInfoTrajOnly(const TrajMeasurement& trajField)
{
	std::cout << "--- Begin traj field info ---" << std::endl;
	std::cout << "validhit:     " << trajField.validhit     << std::endl;;
	std::cout << "missing:      " << trajField.missing      << std::endl;;
	std::cout << "lx:           " << trajField.lx           << std::endl;;
	std::cout << "ly:           " << trajField.ly           << std::endl;;
	std::cout << "lz:           " << trajField.lz           << std::endl;;
	std::cout << "glx:          " << trajField.glx          << std::endl;;
	std::cout << "gly:          " << trajField.gly          << std::endl;;
	std::cout << "glz:          " << trajField.glz          << std::endl;;
	std::cout << "clust_near:   " << trajField.clust_near   << std::endl;;
	std::cout << "hit_near:     " << trajField.hit_near     << std::endl;;
	std::cout << "pass_effcuts: " << trajField.pass_effcuts << std::endl;;
	std::cout << "alpha:        " << trajField.alpha        << std::endl;;
	std::cout << "beta:         " << trajField.beta         << std::endl;;
	std::cout << "norm_charge:  " << trajField.norm_charge  << std::endl;;
	std::cout << "d_tr:         " << trajField.d_tr         << std::endl;;
	std::cout << "dx_tr:        " << trajField.dx_tr        << std::endl;;
	std::cout << "dy_tr:        " << trajField.dy_tr        << std::endl;;
	std::cout << "d_cl:         " << trajField.d_cl         << std::endl;;
	std::cout << "dx_cl:        " << trajField.dx_cl        << std::endl;;
	std::cout << "dy_cl:        " << trajField.dy_cl        << std::endl;;
	std::cout << "dx_hit:       " << trajField.dx_hit       << std::endl;;
	std::cout << "dy_hit:       " << trajField.dy_hit       << std::endl;;
	std::cout << "onedge:       " << trajField.onedge       << std::endl;;
	std::cout << "lx_err:       " << trajField.lx_err       << std::endl;;
	std::cout << "ly_err:       " << trajField.ly_err       << std::endl;;
	std::cout << "--- End traj field info ---"   << std::endl;
}

void printTrajFieldInfo(const TrajMeasurement& trajField)
{
	std::cout << "--- Begin traj field info ---" << std::endl;
	std::cout << "validhit:     " << trajField.validhit    << "." << std::endl;
	std::cout << "missing:      " << trajField.missing    << "." << std::endl;
	std::cout << "lx:           " << trajField.lx    << "." << std::endl;
	std::cout << "ly:           " << trajField.ly    << "." << std::endl;
	std::cout << "lz:           " << trajField.lz    << "." << std::endl;
	std::cout << "glx:          " << trajField.glx    << "." << std::endl;
	std::cout << "gly:          " << trajField.gly    << "." << std::endl;
	std::cout << "glz:          " << trajField.glz    << "." << std::endl;
	std::cout << "clust_near:   " << trajField.clust_near    << "." << std::endl;
	std::cout << "hit_near:     " << trajField.hit_near    << "." << std::endl;
	std::cout << "pass_effcuts: " << trajField.pass_effcuts    << "." << std::endl;
	std::cout << "alpha:        " << trajField.alpha    << "." << std::endl;
	std::cout << "beta:         " << trajField.beta    << "." << std::endl;
	std::cout << "norm_charge:  " << trajField.norm_charge    << "." << std::endl;
	std::cout << "d_tr:         " << trajField.d_tr    << "." << std::endl;
	std::cout << "dx_tr:        " << trajField.dx_tr    << "." << std::endl;
	std::cout << "dy_tr:        " << trajField.dy_tr    << "." << std::endl;
	std::cout << "d_cl:         " << trajField.d_cl    << "." << std::endl;
	std::cout << "dx_cl:        " << trajField.dx_cl    << "." << std::endl;
	std::cout << "dy_cl:        " << trajField.dy_cl    << "." << std::endl;
	std::cout << "dx_hit:       " << trajField.dx_hit    << "." << std::endl;
	std::cout << "dy_hit:       " << trajField.dy_hit    << "." << std::endl;
	std::cout << "onedge:       " << trajField.onedge    << "." << std::endl;
	std::cout << "lx_err:       " << trajField.lx_err    << "." << std::endl;
	std::cout << "ly_err:       " << trajField.ly_err    << "." << std::endl;
	std::cout << "*** Associated module field info ***" << std::endl;
	std::cout << "det:          " << trajField.mod_on.det    << "." << std::endl;
	std::cout << "layer:        " << trajField.mod_on.layer  << "." << std::endl;
	std::cout << "module:       " << trajField.mod_on.module << "." << std::endl;
	std::cout << "ladder:       " << trajField.mod_on.ladder << "." << std::endl;
	std::cout << "det:          " << trajField.mod_on.det    << "." << std::endl;
	std::cout << "*** Associated track field info ***" << std::endl;
	std::cout << "pt:           " << trajField.trk.pt      << "." << std::endl;
	std::cout << "eta:          " << trajField.trk.eta     << "." << std::endl;
	std::cout << "phi:          " << trajField.trk.phi     << "." << std::endl;
	std::cout << "theta:        " << trajField.trk.theta   << "." << std::endl;
	std::cout << "p:            " << trajField.trk.p       << "." << std::endl;
	std::cout << "d0:           " << trajField.trk.d0      << "." << std::endl;
	std::cout << "dz:           " << trajField.trk.dz      << "." << std::endl;
	std::cout << "strip:        " << trajField.trk.strip   << "." << std::endl;
	std::cout << "quality:      " << trajField.trk.quality << "." << std::endl;
	std::cout << "i:            " << trajField.trk.i       << "." << std::endl;
	std::cout << "pix:          " << trajField.trk.pix     << "." << std::endl;
	std::cout << "*** Associated cluster field info ***" << std::endl;
	std::cout << "x:            " << trajField.clu.x             << "." << std::endl;
	std::cout << "y:            " << trajField.clu.y             << "." << std::endl;
	std::cout << "clustSizeX:   " << trajField.clu.sizeX         << "." << std::endl;
	std::cout << "clustSizeY:   " << trajField.clu.sizeY         << "." << std::endl;
	std::cout << "clustIndex:   " << trajField.clu.i  << "." << std::endl;
	std::cout << "charge:       " << trajField.clu.charge        << "." << std::endl;
	std::cout << "--- End traj field info ---" << std::endl;
}

void downscale1DHistogram(TH1D* toDownscale, const TH1D* downscaleFactors)
{
	const auto& nBins = toDownscale -> GetNbinsX();
	if(nBins != downscaleFactors -> GetNbinsX()) throw std::runtime_error(std::string("Error downscaling histograms: ") + toDownscale -> GetName() + ", " + downscaleFactors -> GetName() + " bin numbers do not match.");
	for(int binNum = 0; binNum <= nBins; ++binNum)
	{
		const auto originalBinContent = toDownscale      -> GetBinContent(binNum);
		const auto binDownscaleFactor = downscaleFactors -> GetBinContent(binNum);
		if(binDownscaleFactor == 0) continue;
		// std::cout << "Efficiency bin content before scaling: " << originalBinContent << std::endl;
		toDownscale -> SetBinContent(binNum, originalBinContent / binDownscaleFactor);
		// std::cout << "Efficiency bin content after scaling : " << toDownscale -> GetBinContent(binNum) << std::endl;
	}
}

void downscale2DHistogram(TH2D* toDownscale, const TH2D* downscaleFactors)
{
	const auto& nBinsX = toDownscale -> GetNbinsX();
	const auto& nBinsY = toDownscale -> GetNbinsY();
	if(nBinsX != downscaleFactors -> GetNbinsX()) throw std::runtime_error(std::string("Error downscaling histograms: ") + toDownscale -> GetName() + ", " + downscaleFactors -> GetName() + " x bin numbers do not match.");
	if(nBinsY != downscaleFactors -> GetNbinsY()) throw std::runtime_error(std::string("Error downscaling histograms: ") + toDownscale -> GetName() + ", " + downscaleFactors -> GetName() + " y bin numbers do not match.");
	for(int binXNum = 0; binXNum <= nBinsX; ++binXNum)
	{
		for(int binYNum = 0; binYNum <= nBinsY; ++binYNum)
		{
			const auto originalBinContent = toDownscale      -> GetBinContent(binXNum, binYNum);
			const auto binDownscaleFactor = downscaleFactors -> GetBinContent(binXNum, binYNum);
			if(binDownscaleFactor == 0) continue;
			toDownscale -> SetBinContent(binXNum, binYNum, originalBinContent / binDownscaleFactor);
			if(1 < toDownscale -> GetBinContent(binXNum, binYNum))
			{
				std::cout << "--- Begin bad bin ---" << std::endl;
				std::cout << "Histo to downscale: "         << toDownscale      -> GetName() << ", aka: " << toDownscale      -> GetTitle() << std::endl;
				std::cout << "Downscale factor histogram: " << downscaleFactors -> GetName() << ", aka: " << downscaleFactors -> GetTitle() << std::endl;
				std::cout << "Efficiency bin content before scaling: " << originalBinContent << std::endl;
				std::cout << "Efficiency bin scale factor: "           << binDownscaleFactor << std::endl;
				std::cout << "Efficiency bin content after scaling : " << toDownscale -> GetBinContent(binXNum, binYNum) << std::endl;
				std::cout << "--- End bad bin ---"  << std::endl;
			}
		}
	}
}

#endif // TESTS_COMMON_H
