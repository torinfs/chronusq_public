/* 
 *  This file is part of the Chronus Quantum (ChronusQ) software package
 *  
 *  Copyright (C) 2014-2017 Li Research Group (University of Washington)
 *  
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *  
 *  Contact the Developers:
 *    E-Mail: xsli@uw.edu
 *  
 */
#ifndef __INCLUDED_WAVEFUNCTION_HPP__
#define __INCLUDED_WAVEFUNCTION_HPP__

#include <chronusq_sys.hpp>
#include <quantum.hpp>
#include <wavefunction/base.hpp>

// Debug print triggered by Quantum
  
#ifdef _QuantumDebug
  #define _WaveFunctionDebug
#endif

namespace ChronusQ {


  /**
   *  \brief The WaveFunction class. The typed abstract interface for
   *  all classes which admit a well defined wave function (HF, KS, etc).
   *
   *  Adds knowledge of storage type to WaveFunctionBase
   *
   *  Specializes the Quantum class of the same type; 
   */
  template <typename T>
  class WaveFunction : virtual public WaveFunctionBase, public Quantum<T> {
  protected:

    // Useful typedefs
    typedef T*                   oper_t;
    typedef std::vector<oper_t>  oper_t_coll;

  public:
    
    // Operator storage
    oper_t  mo1;  ///< Full (nC > 1) / ALPHA (nC == 1) MO coefficient matrix
    oper_t  mo2;  ///< BETA (nC == 1) MO coefficient matrix
    double* eps1; ///< Full (nC > 1) / ALPHA (nC == 1) Fock eigenvalues
    double* eps2; ///< BETA (nC == 1) Fock eigenvalues

    // Constructors

    // Disable default constructor
    WaveFunction() = delete;

    /**
     *  WaveFunction Constructor. Constructs a WaveFunction object
     *
     *  \param [in] aoi  AOIntegrals object (which handels the BasisSet, etc)
     *  \param [in] _nC  Number of spin components (1 and 2 are supported)
     *  \param [in] iCS  Whether or not to treat as closed shell
     */ 
    WaveFunction(AOIntegrals &aoi, size_t _nC, bool iCS) :
      QuantumBase(aoi.memManager(),_nC,iCS),
      WaveFunctionBase(aoi,_nC,iCS),
      Quantum<T>(aoi.memManager(),_nC,iCS,aoi.basisSet().nBasis), 
      mo1(nullptr), mo2(nullptr), eps1(nullptr), eps2(nullptr) {

      // Allocate internal memory
      if(aoi.basisSet().nBasis != 0) alloc();

    };

    // See include/wavefunction/impl.hpp for documentation 
    // on the following constructors

    // Different type
    template <typename U> WaveFunction(const WaveFunction<U> &, int dummy = 0);
    template <typename U> WaveFunction(WaveFunction<U> &&     , int dummy = 0);

    // Same type
    WaveFunction(const WaveFunction &);
    WaveFunction(WaveFunction &&);     
    

    /**
     *  Deconstructor
     */ 
    ~WaveFunction(){ dealloc(); }


    // Member functions

    // Deallocation (see include/wavefunction/impl.hpp for docs)
    void alloc();
    void dealloc();

    // Print Functions
    void printMO(std::ostream&) ;
    void printEPS(std::ostream&);

  }; // class WaveFunction

}; // namespace ChronusQ

#endif
