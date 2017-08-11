import Quick
import Nimble

class OCSExceptionSpec: QuickSpec {
    override func spec() {

        describe("An Exception built from an NSException") {
            it("has a string value") {
                let nsException = NSException(
                    name: NSExceptionName(rawValue: "The Name"),
                    reason: "The Reason",
                    userInfo: nil
                )

                let exception = buildOCSException(nsException: nsException)

                expect(exception?.stringValue()).to(equal("__EXCEPTION__:message:<<The Name The Reason>>"))
            }
        }

        func buildOCSException(nsException: NSException) -> OCSException? {
            return OCSException.exception(with: nsException) as? OCSException
        }
    }
    
}
