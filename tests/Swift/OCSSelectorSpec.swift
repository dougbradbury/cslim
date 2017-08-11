import Quick
import Nimble

extension Selector {
    func asString() -> String {
        return NSStringFromSelector(self)
    }
}
class OCSSelectorSpec: QuickSpec {

    override func spec() {
        describe("Building a selector") {
            it("is just the string when there are no args") {
                let selector = OCSSelector.selector(for: "noArgs", andArgs: [])
                expect(selector?.asString()).to(equal("noArgs"))
            }

            it("has a colon at the end of the string when there is one arg") {
                let selector = OCSSelector.selector(for: "withStringArg", andArgs: ["anything"])
                expect(selector?.asString()).to(equal("withStringArg:"))
            }

            it("has just one colon when there are two args, but no delimiters") {
                let args = ["anything", "else"]
                let selector = OCSSelector.selector(for: "withMultipleArgs", andArgs: args)
                expect(selector?.asString()).to(equal("withMultipleArgs:"))
            }

            it("has multiple colons when there are two args and delimiters") {
                let args = ["first", "second", "third"]
                let selector = OCSSelector.selector(for: "three,strings,method", andArgs: args)
                expect(selector?.asString()).to(equal("three:strings:method:"))
            }
        }
    }

}
