# CAD Interoperability

A C++ CAD interoperability project for extracting, representing, and translating structured CAD data.

## Goals

This project explores the architecture and implementation of CAD interoperability pipelines, including:

- CAD geometry and model representation
- File parsing and structured data extraction
- Polymorphic geometry and entity models in modern C++
- Explicit object ownership and lifetime management
- Translation between CAD representations
- Validation of imported and converted data

## Current Status

Early development.

The initial implementation focuses on building a typed geometry import pipeline with:

- `Geometry` as a common base type
- `Line` and `Circle` geometry entities
- Polymorphic model storage
- `std::unique_ptr`-based ownership
- Basic text-based model parsing

## Planned Directions

- STEP entity parsing and reference resolution
- Geometry and topology extraction
- Parts and assemblies
- Parameters and metadata
- Dimensions, constraints, and PMI
- CAD format translation and validation
- Native CAD data extraction experiments
