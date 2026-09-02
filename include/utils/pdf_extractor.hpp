#ifndef PDF_EXTRACTOR_HPP
#define PDF_EXTRACTOR_HPP

#include <string>

// Extracts plain text from a PDF file on disk using pdftotext (poppler).
// Returns empty string on failure.
std::string extractPdfText(const std::string& pdfPath);

#endif