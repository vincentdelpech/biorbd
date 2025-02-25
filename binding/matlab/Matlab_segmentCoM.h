#ifndef BIORBD_MATLAB_SEGMENT_COM_H
#define BIORBD_MATLAB_SEGMENT_COM_H

#include <mex.h>
#include "BiorbdModel.h"
#include "class_handle.h"
#include "processArguments.h"

void Matlab_segmentCOM( int, mxArray *plhs[],
                                int nrhs, const mxArray*prhs[] ){
    // Verifier les arguments d'entrée
    checkNombreInputParametres(nrhs, 3, 4, "3 arguments are required (+1 optional) where the 2nd is the handler on the model, 3rd is the Q and 4th is the index of body segment");

    // Recevoir le model
    biorbd::Model * model = convertMat2Ptr<biorbd::Model>(prhs[1]);
    unsigned int nQ = model->nbQ(); // Get the number of DoF

    // Recevoir Q
    biorbd::rigidbody::GeneralizedCoordinates Q = *getParameterQ(prhs, 2, nQ).begin();
    // Recevoir le numéro du segment (optionnel)
    int i(0);
    if (nrhs==4)
        i = getInteger(prhs, 3);
    i -= 1; // -1 car le segment 0 est numéroté 1 sous matlab

    // Trouver la position du CoM
    if (i==-1){
        std::vector<biorbd::rigidbody::NodeBone> COM = model->CoMbySegment(Q,true);
        // Create a matrix for the return argument
        plhs[0] = mxCreateDoubleMatrix( 3, model->nbBone(), mxREAL);
        // Remplir l'output
        double *tp = mxGetPr(plhs[0]);
        for (unsigned int j=0; j<model->nbBone(); ++j)
                for (unsigned int k=0; k<3; ++k)
                        tp[3*j+k] = COM[j][k]; // Transférer le tout dans un tableau de sortie
    }
    else {
        biorbd::rigidbody::NodeBone COM = model->CoMbySegment(Q,static_cast<unsigned int>(i),true);

        // Create a matrix for the return argument
        plhs[0] = mxCreateDoubleMatrix( 3, 1, mxREAL);
        // Remplir l'output
        double *tp = mxGetPr(plhs[0]);
        for (unsigned int k=0; k<3; ++k)
                tp[k] = COM[k]; // Transférer le tout dans un tableau de sortie
    }

    return;
}

#endif // BIORBD_MATLAB_SEGMENT_COM_H
