/* -*-c++-*- */
/*!
 * \file FilterProcessCallbacks.h
 *
 * \brief Declares the class \c FilterProcessCallbacks.
 */
#ifndef _FILTERPROCESSCALLBACKS_H_
#define _FILTERPROCESSCALLBACKS_H_

#ifdef __GNUG__
# pragma interface
#endif

#include <futil/Process.h>
#include "Datastream.h"

namespace dt {

        /*!
         * \headerfile FilterProcessCallbacks.h "FilterProcessCallbacks.h"
         *
         * \brief The callbacks class used when processing data
         * through filters in the Datastream class.
         */
        class FilterProcessCallbacks : public wml::ProcessCallbacks
        {
        public:
                FilterProcessCallbacks (Datastream* p);
                ~FilterProcessCallbacks ();
                void startedSignal (std::string);
                void errorSignal (int);
                void processFinishedSignal (std::string);
                void readyReadStandardOutputSignal (void);
                void readyReadStandardErrorSignal (void);

        private:
                Datastream * parent;
        };

} // namespace dt

#endif // _FILTERPROCESSCALLBACKS_H_
