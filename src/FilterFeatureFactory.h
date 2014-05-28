/* -*-c++-*- */
/*!
 * \file FilterFeatureFactory.h
 *
 * \brief Declares the class \c FilterFeatureFactory.
 */

#ifndef _FILTERFEATUREFACTORY_H_
#define _FILTERFEATUREFACTORY_H_

#ifdef __GNUG__
# pragma interface
#endif

#include <string>
#include <memory>

namespace dt {

        /*! \name Forward declarations */
        //@{
        class FilterFeature;
        //@}

        /*!
         * \headerfile FilterFeatureFactory.h "FilterFeatureFactory.h"
         *
         * \brief A class holding a factory method for creating a
         * FilterFeature in the Data Transport system.
         */
        class FilterFeatureFactory
        {
        public:

                /*!
                 * \brief Create a smart pointer to a FilterFeature object
                 * with the specified name.
                 *
                 * \note Using std::unique_ptr i.e. assuming single
                 * owner of resource.
                 */
                static std::unique_ptr<FilterFeature> create (const std::string& name);

        private:

        };

} // dt namespace

#endif // _FILTERFEATUREFACTORY_H_
