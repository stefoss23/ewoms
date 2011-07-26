/*****************************************************************************
 *   Copyright (C) 2008-2009 by Markus Wolff                                 *
 *   Copyright (C) 2008-2009 by Andreas Lauser                               *
 *   Copyright (C) 2008 by Bernd Flemisch                                    *
 *   Institute of Hydraulic Engineering                                      *
 *   University of Stuttgart, Germany                                        *
 *   email: <givenname>.<name>@iws.uni-stuttgart.de                          *
 *                                                                           *
 *   This program is free software: you can redistribute it and/or modify    *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation, either version 2 of the License, or       *
 *   (at your option) any later version.                                     *
 *                                                                           *
 *   This program is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *   GNU General Public License for more details.                            *
 *                                                                           *
 *   You should have received a copy of the GNU General Public License       *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 *****************************************************************************/

/*!
 * \ingroup IMPES
 * \ingroup Properties
 */
/*!
 * \file
 *
 * \brief Defines the properties required for (immiscible) twophase sequential models.
 */

#ifndef DUMUX_2PPROPERTIES_DECOUPLED_HH
#define DUMUX_2PPROPERTIES_DECOUPLED_HH

//Dumux-includes
#include <dumux/decoupled/common/impetproperties.hh>
#include <dumux/decoupled/2p/transport/transportproperties.hh>

namespace Dumux
{

////////////////////////////////
// forward declarations
////////////////////////////////

template<class TypeTag>
class VariableClass2P;

template<class TypeTag>
class FluidSystem2P;

template<class TypeTag>
class TwoPFluidState;

template<class TypeTag>
struct TwoPCommonIndicesDecoupled;

////////////////////////////////
// properties
////////////////////////////////
namespace Properties
{
//////////////////////////////////////////////////////////////////
// Type tags
//////////////////////////////////////////////////////////////////

//! The type tag for the two-phase problems
NEW_TYPE_TAG(DecoupledTwoP, INHERITS_FROM(IMPET, Transport))
;

//////////////////////////////////////////////////////////////////
// Property tags
//////////////////////////////////////////////////////////////////

NEW_PROP_TAG ( TwoPIndices )
;
NEW_PROP_TAG( SpatialParameters )
; //!< The type of the spatial parameters object
NEW_PROP_TAG( EnableGravity)
; //!< Returns whether gravity is considered in the problem
NEW_PROP_TAG( PressureFormulation)
; //!< The formulation of the model
NEW_PROP_TAG( SaturationFormulation)
; //!< The formulation of the model
NEW_PROP_TAG( VelocityFormulation)
; //!< The formulation of the model
NEW_PROP_TAG( EnableCompressibility)
;// !< Returns whether compressibility is allowed
NEW_PROP_TAG( WettingPhase)
; //!< The wetting phase for two-phase models
NEW_PROP_TAG( NonwettingPhase)
; //!< The non-wetting phase for two-phase models
NEW_PROP_TAG( FluidSystem )//!< Defines the fluid system
;
NEW_PROP_TAG( FluidState )//!< Defines the fluid state
;

//Properties for linear solvers
NEW_PROP_TAG(PressureCoefficientMatrix);//!< Type of the coefficient matrix given to the linear solver
NEW_PROP_TAG(PressureRHSVector);//!< Type of the right hand side vector given to the linear solver
NEW_PROP_TAG( PressurePreconditioner );//!< Type of the pressure preconditioner
NEW_PROP_TAG( PressureSolver );//!< Type of the pressure solver
NEW_PROP_TAG( SolverParameters );//!< Container for the solver parameters
NEW_PROP_TAG(ReductionSolver);//!< Reduction value for linear solver
NEW_PROP_TAG(MaxIterationNumberSolver);//!< Maximum iteration number for linear solver
NEW_PROP_TAG(IterationNumberPreconditioner);//!< Iteration number for preconditioner
NEW_PROP_TAG(VerboseLevelSolver);//!<Verbose level of linear solver and preconditioner
NEW_PROP_TAG(RelaxationPreconditioner);//!< Relaxation factor for preconditioner

//////////////////////////////////////////////////////////////////
// Properties
//////////////////////////////////////////////////////////////////

SET_INT_PROP(DecoupledTwoP, NumEq, 2);

SET_INT_PROP(DecoupledTwoP, NumPhases, 2);//!< The number of phases in the 2p model is 2

SET_INT_PROP(DecoupledTwoP, NumComponents, 1); //!< Each phase consists of 1 pure component


SET_PROP(DecoupledTwoP, TwoPIndices)
{
typedef TwoPCommonIndicesDecoupled<TypeTag> type;
};

//! Set the default formulation
SET_INT_PROP(DecoupledTwoP,
    PressureFormulation,
    TwoPCommonIndicesDecoupled<TypeTag>::pressureW);

SET_INT_PROP(DecoupledTwoP,
    SaturationFormulation,
    TwoPCommonIndicesDecoupled<TypeTag>::saturationW);

SET_INT_PROP(DecoupledTwoP,
    VelocityFormulation,
    TwoPCommonIndicesDecoupled<TypeTag>::velocityTotal);

SET_BOOL_PROP(DecoupledTwoP, EnableCompressibility, false);

SET_TYPE_PROP(DecoupledTwoP, Variables, VariableClass2P<TypeTag>);

SET_TYPE_PROP(DecoupledTwoP, FluidSystem, FluidSystem2P<TypeTag>);

SET_TYPE_PROP(DecoupledTwoP, FluidState, TwoPFluidState<TypeTag>);

SET_PROP_DEFAULT(LocalStiffness)
{
private:
    typedef typename GET_PROP_TYPE(TypeTag, PTAG(GridView)) GridView;
    typedef typename GET_PROP_TYPE(TypeTag, PTAG(Scalar)) Scalar;
    typedef typename GET_PROP_TYPE(TypeTag, PTAG(GridView)) Variables;
    typedef typename GET_PROP_TYPE(TypeTag, PTAG(Problem)) Problem;

public:
    typedef MimeticGroundwaterEquationLocalStiffness<GridView,Scalar,Variables, Problem> type;
};
// \}
}

/*!
 * \brief The common indices for the two-phase model.
 */
template <class TypeTag>
struct TwoPCommonIndicesDecoupled
{
private:
typedef typename GET_PROP_TYPE(TypeTag, PTAG(FluidSystem)) FluidSystem;

public:
//equation idx
static const int pressureEq = 0;
static const int saturationEq = 1;
// Formulations
//saturation flags
static const int saturationW = 0;
static const int saturationNW = 1;
//pressure flags
static const int pressureW = 0;
static const int pressureNW = 1;
static const int pressureGlobal = 2;
//velocity flags
static const int velocityW = 0;
static const int velocityNW = 1;
static const int velocityTotal = 2;

// Phase indices
static const int wPhaseIdx = FluidSystem::wPhaseIdx; //!< Index of the wetting phase in a phase vector
static const int nPhaseIdx = FluidSystem::nPhaseIdx; //!< Index of the non-wetting phase in a phase vector
};


}

#endif
