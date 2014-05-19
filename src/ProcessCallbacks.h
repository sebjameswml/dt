/* -*-c++-*- */
/*!
 * \file ProcessCallbacks.h
 *
 * \brief Declares the class \c ProcessCallbacks.
 */
#ifndef _DT_PROCESSCALLBACKS_H_
#define _DT_PROCESSCALLBACKS_H_

#ifdef __GNUG__
# pragma interface
#endif

#include <functional>
#include <futil/Process.h>

namespace dt {

        /*!
         * \headerfile ProcessCallbacks.h "ProcessCallbacks.h"
         *
         * \brief An extended callbacks class that simplifies the
         * process of defining callbacks for a particular use case.
         *
         * \c ProcessCallbacks allows the client to specify
         * callable objects for use in handling particular
         * signals. These objects, if specified, are invoked with the
         * appropriate parameters when the callback is called.
         *
         * This simplifies and generalizes the process of defining
         * callbacks since all that is required is to bind an object,
         * which may be a static or non-static member function, a
         * lambda expression, or some other callable object, to any
         * callback for which functionality is desired.
         */
        class ProcessCallbacks : public wml::ProcessCallbacks
        {
        public:
                ProcessCallbacks();

                /*!
                 * \note Destructor declared virtual to ensure proper
                 * cleanup of derived class objects.
                 */
                virtual ~ProcessCallbacks();

                /*!
                 * \name Callback invocations
                 *
                 * Invoke member callback objects to handle signals of
                 * various types. If a member callback has not been
                 * assigned then simply output some basic debugging
                 * information.
                 */
                //@{
                void startedSignal (string);
                void errorSignal (int);
                void returnCodeSignal (int);
                void processFinishedSignal (string);
                void readyReadStandardOutputSignal (void);
                void readyReadStandardErrorSignal (void);
                //@}

                /*!
                 * \name Callback assignment
                 */
                //@{
                void addStartedSignalCallback (std::function<void(string)>);
                void addErrorSignalCallback (std::function<void(int)>);
                void addReturnCodeSignalCallback (std::function<void(int)>);
                void addProcessFinishedSignalCallback (std::function<void(string)>);
                void addReadStdoutSignalCallback (std::function<void()>);
                void addReadStderrSignalCallback (std::function<void()>);
                //@}

                /*!
                 * \brief Clear all current callbacks.
                 */
                void clear (void);

        private:
                std::function<void(string)> startedSignalCallback;
                std::function<void(int)> errorSignalCallback;
                std::function<void(int)> returnCodeSignalCallback;
                std::function<void(string)> processFinishedSignalCallback;
                std::function<void(void)> readStdoutSignalCallback;
                std::function<void(void)> readStderrSignalCallback;

        };

} // namespace dt

#endif // _DT_PROCESSCALLBACKS_H_
