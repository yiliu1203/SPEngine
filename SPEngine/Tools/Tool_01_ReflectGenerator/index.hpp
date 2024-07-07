#include "clang-c/Index.h"

namespace flection {
	class index {
	public:
		index(int excludeDeclarationsFromPCH, int displayDiagnostics)
			: _index(clang_createIndex(excludeDeclarationsFromPCH,
				displayDiagnostics)) {}
		inline const CXIndex& getIndex() { return this->_index; }
		~index() { }

	private:
		CXIndex _index;
	};

} // namespace flection