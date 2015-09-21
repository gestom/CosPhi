% Intrinsic and Extrinsic Camera Parameters
%
% This script file can be directly excecuted under Matlab to recover the camera intrinsic and extrinsic parameters.
% IMPORTANT: This file contains neither the structure of the calibration objects nor the image coordinates of the calibration points.
%            All those complementary variables are saved in the complete matlab data file Calib_Results.mat.
% For more information regarding the calibration model visit http://www.vision.caltech.edu/bouguetj/calib_doc/


%-- Focal length:
fc = [ 2046.335597231528254 ; 2044.664866517985502 ];

%-- Principal point:
cc = [ 966.557685055673005 ; 542.342373298518737 ];

%-- Skew coefficient:
alpha_c = 0.000000000000000;

%-- Distortion coefficients:
kc = [ -0.205453636719254 ; 0.191642407160967 ; 0.001959209100130 ; -0.001967025421466 ; 0.000000000000000 ];

%-- Focal length uncertainty:
fc_error = [ 7.499093690778241 ; 7.180837323148110 ];

%-- Principal point uncertainty:
cc_error = [ 11.068419969130591 ; 10.378307138329518 ];

%-- Skew coefficient uncertainty:
alpha_c_error = 0.000000000000000;

%-- Distortion coefficients uncertainty:
kc_error = [ 0.012403880365008 ; 0.061525224738211 ; 0.001030773270688 ; 0.000986044381546 ; 0.000000000000000 ];

%-- Image size:
nx = 1920;
ny = 1080;


%-- Various other variables (may be ignored if you do not use the Matlab Calibration Toolbox):
%-- Those variables are used to control which intrinsic parameters should be optimized

n_ima = 15;						% Number of calibration images
est_fc = [ 1 ; 1 ];					% Estimation indicator of the two focal variables
est_aspect_ratio = 1;				% Estimation indicator of the aspect ratio fc(2)/fc(1)
center_optim = 1;					% Estimation indicator of the principal point
est_alpha = 0;						% Estimation indicator of the skew coefficient
est_dist = [ 1 ; 1 ; 1 ; 1 ; 0 ];	% Estimation indicator of the distortion coefficients


%-- Extrinsic parameters:
%-- The rotation (omc_kk) and the translation (Tc_kk) vectors for every calibration image and their uncertainties

%-- Image #1:
omc_1 = [ -2.196690e+00 ; -2.183761e+00 ; 7.220564e-02 ];
Tc_1  = [ -1.427960e+02 ; -8.794399e+01 ; 4.016699e+02 ];
omc_error_1 = [ 3.958516e-03 ; 3.637526e-03 ; 8.099262e-03 ];
Tc_error_1  = [ 2.196213e+00 ; 2.047560e+00 ; 1.757186e+00 ];

%-- Image #2:
omc_2 = [ -1.973731e+00 ; -1.945289e+00 ; 5.775772e-01 ];
Tc_2  = [ -1.515909e+02 ; -7.066145e+01 ; 4.670224e+02 ];
omc_error_2 = [ 4.533219e-03 ; 3.536366e-03 ; 7.239716e-03 ];
Tc_error_2  = [ 2.530318e+00 ; 2.361352e+00 ; 1.930417e+00 ];

%-- Image #3:
omc_3 = [ 1.874187e+00 ; 1.822366e+00 ; -6.420502e-01 ];
Tc_3  = [ -1.371286e+02 ; -6.492013e+01 ; 4.345921e+02 ];
omc_error_3 = [ 3.095974e-03 ; 4.665631e-03 ; 7.047169e-03 ];
Tc_error_3  = [ 2.361956e+00 ; 2.191107e+00 ; 1.848471e+00 ];

%-- Image #4:
omc_4 = [ 2.468693e+00 ; 1.856561e-03 ; 1.155187e-01 ];
Tc_4  = [ -1.172131e+02 ; 4.183067e+01 ; 3.111980e+02 ];
omc_error_4 = [ 4.873694e-03 ; 2.635854e-03 ; 7.169717e-03 ];
Tc_error_4  = [ 1.686806e+00 ; 1.615369e+00 ; 1.398534e+00 ];

%-- Image #5:
omc_5 = [ 2.306215e+00 ; -1.629043e-02 ; 8.350825e-02 ];
Tc_5  = [ -1.310747e+02 ; 6.813191e+01 ; 3.625598e+02 ];
omc_error_5 = [ 4.962711e-03 ; 2.866911e-03 ; 6.570380e-03 ];
Tc_error_5  = [ 1.976679e+00 ; 1.890876e+00 ; 1.665032e+00 ];

%-- Image #6:
omc_6 = [ 1.981634e+00 ; 1.096895e+00 ; -3.455910e-01 ];
Tc_6  = [ -1.627137e+02 ; -1.241008e+01 ; 5.022671e+02 ];
omc_error_6 = [ 4.446938e-03 ; 4.046952e-03 ; 6.331015e-03 ];
Tc_error_6  = [ 2.725134e+00 ; 2.563962e+00 ; 2.193620e+00 ];

%-- Image #7:
omc_7 = [ 1.878394e+00 ; 1.372722e+00 ; -4.451734e-01 ];
Tc_7  = [ -1.515896e+02 ; -4.144807e+01 ; 4.970273e+02 ];
omc_error_7 = [ 4.089694e-03 ; 4.407559e-03 ; 6.433105e-03 ];
Tc_error_7  = [ 2.695948e+00 ; 2.524006e+00 ; 2.127981e+00 ];

%-- Image #8:
omc_8 = [ 2.079419e+00 ; 6.992546e-01 ; -1.611972e-01 ];
Tc_8  = [ -2.084330e+02 ; -2.006783e+01 ; 5.531550e+02 ];
omc_error_8 = [ 4.933916e-03 ; 3.660084e-03 ; 6.234279e-03 ];
Tc_error_8  = [ 2.999252e+00 ; 2.836226e+00 ; 2.486924e+00 ];

%-- Image #9:
omc_9 = [ 2.169217e+00 ; 3.837484e-02 ; 3.546126e-02 ];
Tc_9  = [ -1.387389e+02 ; 4.307518e+01 ; 4.084072e+02 ];
omc_error_9 = [ 5.032991e-03 ; 3.168631e-03 ; 6.156313e-03 ];
Tc_error_9  = [ 2.220621e+00 ; 2.109721e+00 ; 1.830241e+00 ];

%-- Image #10:
omc_10 = [ 3.048781e+00 ; 1.705705e-02 ; -7.170718e-02 ];
Tc_10  = [ -1.624009e+02 ; 6.928609e+01 ; 4.964237e+02 ];
omc_error_10 = [ 5.939229e-03 ; 1.189555e-03 ; 1.026095e-02 ];
Tc_error_10  = [ 2.699022e+00 ; 2.557587e+00 ; 2.151073e+00 ];

%-- Image #11:
omc_11 = [ 3.009206e+00 ; 1.392031e-01 ; -1.069565e-01 ];
Tc_11  = [ -1.813549e+02 ; 7.678103e+01 ; 5.045075e+02 ];
omc_error_11 = [ 5.736343e-03 ; 1.362471e-03 ; 1.002605e-02 ];
Tc_error_11  = [ 2.744929e+00 ; 2.609661e+00 ; 2.220411e+00 ];

%-- Image #12:
omc_12 = [ 2.077416e+00 ; -2.316559e-02 ; -1.057547e-01 ];
Tc_12  = [ -1.352396e+02 ; 4.179063e+01 ; 2.950151e+02 ];
omc_error_12 = [ 4.990985e-03 ; 3.371314e-03 ; 5.772683e-03 ];
Tc_error_12  = [ 1.604661e+00 ; 1.534242e+00 ; 1.404847e+00 ];

%-- Image #13:
omc_13 = [ 2.083616e+00 ; 2.137698e-02 ; -1.355037e-01 ];
Tc_13  = [ -1.262761e+02 ; 5.613646e+01 ; 3.157359e+02 ];
omc_error_13 = [ 4.940866e-03 ; 3.258296e-03 ; 5.790358e-03 ];
Tc_error_13  = [ 1.724498e+00 ; 1.641950e+00 ; 1.435324e+00 ];

%-- Image #14:
omc_14 = [ 2.225009e+00 ; 1.404605e-02 ; 7.921705e-02 ];
Tc_14  = [ -1.261096e+02 ; 2.786252e+01 ; 3.021664e+02 ];
omc_error_14 = [ 4.977476e-03 ; 3.094895e-03 ; 6.258667e-03 ];
Tc_error_14  = [ 1.640410e+00 ; 1.560589e+00 ; 1.428523e+00 ];

%-- Image #15:
omc_15 = [ 2.448236e+00 ; -2.597044e-01 ; 1.904125e-01 ];
Tc_15  = [ -1.421062e+02 ; 3.928144e+01 ; 5.555437e+02 ];
omc_error_15 = [ 5.246758e-03 ; 2.858768e-03 ; 7.474143e-03 ];
Tc_error_15  = [ 3.004872e+00 ; 2.855140e+00 ; 2.343333e+00 ];

