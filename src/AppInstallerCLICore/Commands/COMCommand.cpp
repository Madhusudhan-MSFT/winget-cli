// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
#include "pch.h"
#include "COMCommand.h"
#include "Workflows/DownloadFlow.h"
#include "Workflows/InstallFlow.h"
#include "Workflows/PromptFlow.h"
#include "Workflows/UninstallFlow.h"
#include "Workflows/WorkflowBase.h"
#include "Workflows/DependenciesFlow.h"

namespace AppInstaller::CLI
{
    using namespace AppInstaller::CLI::Execution;
    using namespace AppInstaller::CLI::Workflow;
    using namespace AppInstaller::Manifest;
    using namespace AppInstaller::Utility::literals;

    // IMPORTANT: To use this command, the caller should have already retrieved the package manifest (GetManifest()) and added it to the Context Data
    void COMDownloadCommand::ExecuteInternal(Context& context) const
    {
        context <<
            Workflow::ReportExecutionStage(ExecutionStage::Discovery) <<
            Workflow::SelectInstaller <<
            Workflow::EnsureApplicableInstaller <<
            Workflow::ReportIdentityAndInstallationDisclaimer <<
            Workflow::ShowPromptsForSinglePackage(/* ensureAcceptance */ true) <<
            Workflow::SetDownloadDirectory <<
            Workflow::DownloadPackageDependencies <<
            Workflow::DownloadInstaller;
    }

    void COMDownloadCommand::Execute(Context& context) const
    {
        context.SetFlags(Execution::ContextFlag::WinGetCOMApiCall);
        Command::Execute(context);
    }

    // IMPORTANT: To use this command, the caller should have already executed the COMDownloadCommand
    void COMInstallCommand::ExecuteInternal(Context& context) const
    {
        context <<
            Workflow::InstallDependencies <<
            Workflow::ReverifyInstallerHash << 
            Workflow::InstallPackageInstaller;
    }

    void COMInstallCommand::Execute(Context& context) const
    {
        context.SetFlags(Execution::ContextFlag::WinGetCOMApiCall);
        Command::Execute(context);
    }

    // IMPORTANT: To use this command, the caller should have already retrieved the InstalledPackageVersion and added it to the Context Data
    void COMUninstallCommand::ExecuteInternal(Execution::Context& context) const
    {
        context <<
            Workflow::UninstallSinglePackage;
    }

    void COMUninstallCommand::Execute(Execution::Context& context) const
    {
        context.SetFlags(Execution::ContextFlag::WinGetCOMApiCall);
        Command::Execute(context);
    }
}
