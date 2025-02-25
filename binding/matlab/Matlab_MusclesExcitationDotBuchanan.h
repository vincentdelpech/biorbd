#ifndef BIORBD_MATLAB_MUSCLES_EXCITATION_DOT_BUCHANAN_H
#define BIORBD_MATLAB_MUSCLES_EXCITATION_DOT_BUCHANAN_H

#include <mex.h>
#include "BiorbdModel.h"
#include "class_handle.h"
#include "processArguments.h"
#include "Muscles/Muscle.h"
#include "Muscles/MuscleGroup.h"

void Matlab_MusclesExcitationDotBuchanan( int, mxArray *plhs[],
                  int nrhs, const mxArray*prhs[] ){

    // Verifier les arguments d'entrée
    checkNombreInputParametres(nrhs, 5, 5, "4 arguments are required [+1 optional] where the 2nd is the handler on the model, 3rd is the excitation, 4th is the activation and 5th is a bool to express if excitation is already normalized");
    // Recevoir le model
    biorbd::Model * model = convertMat2Ptr<biorbd::Model>(prhs[1]);

    // Recevoir les états musculaires
    std::vector<std::vector<biorbd::muscles::StateDynamicsBuchanan>> state = getParameterMuscleStateBuchanan(prhs, 2, 3, model->nbMuscleTotal());

    // Already normalized
    bool alreadyNormalized(false);
    if (nrhs > 4)
        alreadyNormalized = static_cast<bool>(*(mxGetPr(prhs[4])));

    // Cellules de sortie
    mwSize dims[2];
    dims[0] = model->nbMuscleTotal();
    dims[1] = state.size();
    plhs[0] = mxCreateNumericArray(2, dims, mxDOUBLE_CLASS, mxREAL);
    double *edot = mxGetPr(plhs[0]);

    // Aller chercher les valeurs
    unsigned int cmp(0);
    for (unsigned int iTime=0; iTime<state.size(); ++iTime){
        bool updateKin(true);
        unsigned int cmpState(0);
        for (unsigned int i=0; i<model->nbMuscleGroups(); ++i)
            for (unsigned int j=0; j<model->muscleGroup(i).nbMuscles(); ++j){
                 // Recueillir dérivées d'excitations
                double shapeFactor = dynamic_cast<const biorbd::muscles::StateDynamicsBuchanan&>( model->muscleGroup(i).muscle(j).state() ).shapeFactor();
                (*((*(state.begin()+iTime)).begin() + cmpState)).shapeFactor(shapeFactor);
                edot[cmp] = (*((*(state.begin()+iTime)).begin() + cmpState)).timeDerivativeExcitation(model->muscleGroup(i).muscle(j).caract(), true);
                updateKin = false;
                ++cmp;
                ++cmpState;
            }
    }

    return;
}

#endif // BIORBD_MATLAB_MUSCLES_EXCITATION_DOT_BUCHANAN_H
