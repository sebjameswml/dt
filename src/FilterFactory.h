/* -*-c++-*- */
/*!
 * \file FilterFactory.h
 *
 * \brief Declares the class \c FilterFactory.
 */

#ifndef _FILTERFACTORY_H_
#define _FILTERFACTORY_H_

#ifdef __GNUG__
# pragma interface
#endif

#include <string>
#include <memory>

#include "ExtProcessCallbacks.h"

namespace dt {

        /*! \name Forward declarations */
        //@{
        class BaseFilter;
        //@}

        /*!
         * \headerfile FilterFactory.h "FilterFactory.h"
         *
         * \brief A class holding a factory method for creating a
         * filter in the Data Transport system.
         */
        class FilterFactory
        {
        public:

                /*!
                 * \brief Create a smart pointer to a filter object
                 * with the specified path.
                 *
                 * \note Using std::unique_ptr i.e. assuming single
                 * owner of resource.
                 */
                static std::unique_ptr<BaseFilter> create (const std::string& path);

                /*!
                 * \brief Store the specified return code to
                 * FilterFactory::returnCode.
                 */
                static void storeReturnCode (int i);

                /*!
                 * \brief Return code, used to store the result of
                 * calling a filter.
                 */
                static int returnCode;

        private:

        };

        /*!
         * \headerfile FilterFactoryCallbacks.h "FilterFactoryCallbacks.h"
         *
         * \brief Class for specifying callbacks for querying filters
         * in the FilterFactory::create() method.
         */
        class FilterFactoryCallbacks : public ExtProcessCallbacks
        {
        public:
                FilterFactoryCallbacks();

        };

} // dt namespace

#endif // _FILTERFACTORY_H_
