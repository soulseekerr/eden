// main.cpp
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

#include <fstream>

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;

std::ofstream logFile("matcher_output.txt");

class StringReturnPrinter : public MatchFinder::MatchCallback {
public:
    void run(const MatchFinder::MatchResult &Result) override {
        if (const FunctionDecl *FD = Result.Nodes.getNodeAs<FunctionDecl>("stringReturnFunc")) {
            const auto& SM = Result.Context->getSourceManager();
            // if (!SM.isWrittenInMainFile(FD->getLocation())) return;
            std::string filename = SM.getFilename(FD->getLocation()).str();
            if (filename.find("/eden/") == std::string::npos) return;

            FullSourceLoc FullLocation = Result.Context->getFullLoc(FD->getBeginLoc());
            if (FullLocation.isValid()) {
                llvm::outs() << filename << "- Function returning std::string found at "
                             << FullLocation.getSpellingLineNumber() << ":"
                             << FullLocation.getSpellingColumnNumber() << ": "
                             << FD->getNameAsString() << "\n";
                
                logFile << filename << "- Function returning std::string found at "
                        << FullLocation.getSpellingLineNumber() << ":"
                        << FullLocation.getSpellingColumnNumber() << ": "
                        << FD->getNameAsString() << "\n";
            }
        }
    }
};

class MutableFieldPrinter : public MatchFinder::MatchCallback {
public:
    void run(const MatchFinder::MatchResult &Result) override {
        if (const FieldDecl *FD = Result.Nodes.getNodeAs<FieldDecl>("field")) {
            const auto& SM = Result.Context->getSourceManager();
            // if (!SM.isWrittenInMainFile(FD->getLocation())) return;
            std::string filename = SM.getFilename(FD->getLocation()).str();
            if (filename.find("/eden/") == std::string::npos) return;
        
            if (FD->isMutable()) {
                FullSourceLoc FullLocation = Result.Context->getFullLoc(FD->getBeginLoc());
                if (FullLocation.isValid()) {
                    llvm::outs() << filename << "- Mutable field found at "
                                 << FullLocation.getSpellingLineNumber() << ":"
                                 << FullLocation.getSpellingColumnNumber() << ": "
                                 << FD->getNameAsString() << "\n";
                    
                    logFile << filename << "- Mutable field found at "
                            << FullLocation.getSpellingLineNumber() << ":"
                            << FullLocation.getSpellingColumnNumber() << ": "
                            << FD->getNameAsString() << "\n";
                }
            }
        }
    }
};

class SharedPtrFieldPrinter : public MatchFinder::MatchCallback {
public:
    void run(const MatchFinder::MatchResult &Result) override {
        if (const FieldDecl *FD = Result.Nodes.getNodeAs<FieldDecl>("sharedPtrField")) {
            const auto& SM = Result.Context->getSourceManager();
            std::string filename = SM.getFilename(FD->getLocation()).str();
            if (filename.find("/eden/") == std::string::npos) return;

            FullSourceLoc FullLocation = Result.Context->getFullLoc(FD->getBeginLoc());
            if (FullLocation.isValid()) {
                llvm::outs() << filename << "- Field using std::shared_ptr found at "
                             << FullLocation.getSpellingLineNumber() << ":"
                             << FullLocation.getSpellingColumnNumber() << ": "
                             << FD->getNameAsString() << "\n";

                logFile << filename << "- Field using std::shared_ptr found at "
                        << FullLocation.getSpellingLineNumber() << ":"
                        << FullLocation.getSpellingColumnNumber() << ": "
                        << FD->getNameAsString() << "\n";
            }
        }
    }
};

class NonConstRefParamPrinter : public MatchFinder::MatchCallback {
public:
    void run(const MatchFinder::MatchResult &Result) override {
        if (const ParmVarDecl *PD = Result.Nodes.getNodeAs<ParmVarDecl>("nonConstRef")) {
            const auto& SM = Result.Context->getSourceManager();
            std::string filename = SM.getFilename(PD->getLocation()).str();
            if (filename.find("/eden/") == std::string::npos) return;

            FullSourceLoc FullLocation = Result.Context->getFullLoc(PD->getBeginLoc());
            if (FullLocation.isValid()) {
                llvm::outs() << filename << "- Non-const reference parameter found at "
                             << FullLocation.getSpellingLineNumber() << ":"
                             << FullLocation.getSpellingColumnNumber() << ": "
                             << PD->getNameAsString() << "\n";

                logFile << filename << "- Non-const reference parameter found at "
                        << FullLocation.getSpellingLineNumber() << ":"
                        << FullLocation.getSpellingColumnNumber() << ": "
                        << PD->getNameAsString() << "\n";
            }
        }
    }
};

int main(int argc, const char **argv) {
    llvm::cl::OptionCategory MyToolCategory("my-tool options");
    auto ExpectedParser = CommonOptionsParser::create(argc, argv, MyToolCategory);
    if (!ExpectedParser) {
        llvm::errs() << ExpectedParser.takeError();
        return 1;
    }
    CommonOptionsParser &OptionsParser = ExpectedParser.get();
    ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());

    MatchFinder Finder;

    StringReturnPrinter stringPrinter;
    auto stringMatcher = functionDecl(returns(asString("std::string"))).bind("stringReturnFunc");
    Finder.addMatcher(stringMatcher, &stringPrinter);

    MutableFieldPrinter mutablePrinter;
    auto mutableMatcher = fieldDecl().bind("field");
    Finder.addMatcher(mutableMatcher, &mutablePrinter);

    SharedPtrFieldPrinter sharedPrinter;
    auto sharedPtrMatcher = fieldDecl(hasType(qualType(hasDeclaration(recordDecl(hasName("std::shared_ptr")))))).bind("sharedPtrField");
    Finder.addMatcher(sharedPtrMatcher, &sharedPrinter);

    NonConstRefParamPrinter refPrinter;
    auto nonConstRefMatcher = parmVarDecl(hasType(referenceType(unless(pointee(isConstQualified()))))).bind("nonConstRef");
    Finder.addMatcher(nonConstRefMatcher, &refPrinter);

    auto res = Tool.run(newFrontendActionFactory(&Finder).get());

    logFile.close();

    return res;
}
