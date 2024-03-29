// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_SIGMFSIGNAL_SIGMF_SIGNAL_H_
#define FLATBUFFERS_GENERATED_SIGMFSIGNAL_SIGMF_SIGNAL_H_

#include "flatbuffers/flatbuffers.h"

#include "sigmf_core_generated.h"

namespace sigmf {
namespace signal {

struct Global;
struct GlobalBuilder;
struct GlobalT;

struct Capture;
struct CaptureBuilder;
struct CaptureT;

struct signal_detail;
struct signal_detailBuilder;
struct signal_detailT;

struct signal_emitter;
struct signal_emitterBuilder;
struct signal_emitterT;

struct Annotation;
struct AnnotationBuilder;
struct AnnotationT;

struct Descr;
struct DescrBuilder;
struct DescrT;

inline const flatbuffers::TypeTable *GlobalTypeTable();

inline const flatbuffers::TypeTable *CaptureTypeTable();

inline const flatbuffers::TypeTable *signal_detailTypeTable();

inline const flatbuffers::TypeTable *signal_emitterTypeTable();

inline const flatbuffers::TypeTable *AnnotationTypeTable();

inline const flatbuffers::TypeTable *DescrTypeTable();

struct GlobalT : public flatbuffers::NativeTable {
  typedef Global TableType;
};

struct Global FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef GlobalT NativeTableType;
  typedef GlobalBuilder Builder;
  static const flatbuffers::TypeTable *MiniReflectTypeTable() {
    return GlobalTypeTable();
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           verifier.EndTable();
  }
  GlobalT *UnPack(const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(GlobalT *_o, const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static flatbuffers::Offset<Global> Pack(flatbuffers::FlatBufferBuilder &_fbb, const GlobalT* _o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct GlobalBuilder {
  typedef Global Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  explicit GlobalBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<Global> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Global>(end);
    return o;
  }
};

inline flatbuffers::Offset<Global> CreateGlobal(
    flatbuffers::FlatBufferBuilder &_fbb) {
  GlobalBuilder builder_(_fbb);
  return builder_.Finish();
}

flatbuffers::Offset<Global> CreateGlobal(flatbuffers::FlatBufferBuilder &_fbb, const GlobalT *_o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);

struct CaptureT : public flatbuffers::NativeTable {
  typedef Capture TableType;
};

struct Capture FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef CaptureT NativeTableType;
  typedef CaptureBuilder Builder;
  static const flatbuffers::TypeTable *MiniReflectTypeTable() {
    return CaptureTypeTable();
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           verifier.EndTable();
  }
  CaptureT *UnPack(const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(CaptureT *_o, const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static flatbuffers::Offset<Capture> Pack(flatbuffers::FlatBufferBuilder &_fbb, const CaptureT* _o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct CaptureBuilder {
  typedef Capture Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  explicit CaptureBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<Capture> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Capture>(end);
    return o;
  }
};

inline flatbuffers::Offset<Capture> CreateCapture(
    flatbuffers::FlatBufferBuilder &_fbb) {
  CaptureBuilder builder_(_fbb);
  return builder_.Finish();
}

flatbuffers::Offset<Capture> CreateCapture(flatbuffers::FlatBufferBuilder &_fbb, const CaptureT *_o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);

struct signal_detailT : public flatbuffers::NativeTable {
  typedef signal_detail TableType;
  std::string type{};
  std::string mod_class{};
  std::string standard{};
  std::string carrier_variant{};
  std::string symbol_variant{};
  flatbuffers::Optional<uint64_t> order = flatbuffers::nullopt;
  std::string duplexing{};
  std::string multiplexing{};
  std::string multiple_access{};
  std::string spreading{};
  flatbuffers::Optional<double> channel_bw = flatbuffers::nullopt;
  flatbuffers::Optional<uint64_t> channel = flatbuffers::nullopt;
  std::string class_variant{};
};

struct signal_detail FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef signal_detailT NativeTableType;
  typedef signal_detailBuilder Builder;
  static const flatbuffers::TypeTable *MiniReflectTypeTable() {
    return signal_detailTypeTable();
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_TYPE = 4,
    VT_MOD_CLASS = 6,
    VT_STANDARD = 8,
    VT_CARRIER_VARIANT = 10,
    VT_SYMBOL_VARIANT = 12,
    VT_ORDER = 14,
    VT_DUPLEXING = 16,
    VT_MULTIPLEXING = 18,
    VT_MULTIPLE_ACCESS = 20,
    VT_SPREADING = 22,
    VT_CHANNEL_BW = 24,
    VT_CHANNEL = 26,
    VT_CLASS_VARIANT = 28
  };
  const flatbuffers::String *type() const {
    return GetPointer<const flatbuffers::String *>(VT_TYPE);
  }
  const flatbuffers::String *mod_class() const {
    return GetPointer<const flatbuffers::String *>(VT_MOD_CLASS);
  }
  const flatbuffers::String *standard() const {
    return GetPointer<const flatbuffers::String *>(VT_STANDARD);
  }
  const flatbuffers::String *carrier_variant() const {
    return GetPointer<const flatbuffers::String *>(VT_CARRIER_VARIANT);
  }
  const flatbuffers::String *symbol_variant() const {
    return GetPointer<const flatbuffers::String *>(VT_SYMBOL_VARIANT);
  }
  flatbuffers::Optional<uint64_t> order() const {
    return GetOptional<uint64_t, uint64_t>(VT_ORDER);
  }
  const flatbuffers::String *duplexing() const {
    return GetPointer<const flatbuffers::String *>(VT_DUPLEXING);
  }
  const flatbuffers::String *multiplexing() const {
    return GetPointer<const flatbuffers::String *>(VT_MULTIPLEXING);
  }
  const flatbuffers::String *multiple_access() const {
    return GetPointer<const flatbuffers::String *>(VT_MULTIPLE_ACCESS);
  }
  const flatbuffers::String *spreading() const {
    return GetPointer<const flatbuffers::String *>(VT_SPREADING);
  }
  flatbuffers::Optional<double> channel_bw() const {
    return GetOptional<double, double>(VT_CHANNEL_BW);
  }
  flatbuffers::Optional<uint64_t> channel() const {
    return GetOptional<uint64_t, uint64_t>(VT_CHANNEL);
  }
  const flatbuffers::String *class_variant() const {
    return GetPointer<const flatbuffers::String *>(VT_CLASS_VARIANT);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_TYPE) &&
           verifier.VerifyString(type()) &&
           VerifyOffset(verifier, VT_MOD_CLASS) &&
           verifier.VerifyString(mod_class()) &&
           VerifyOffset(verifier, VT_STANDARD) &&
           verifier.VerifyString(standard()) &&
           VerifyOffset(verifier, VT_CARRIER_VARIANT) &&
           verifier.VerifyString(carrier_variant()) &&
           VerifyOffset(verifier, VT_SYMBOL_VARIANT) &&
           verifier.VerifyString(symbol_variant()) &&
           VerifyField<uint64_t>(verifier, VT_ORDER) &&
           VerifyOffset(verifier, VT_DUPLEXING) &&
           verifier.VerifyString(duplexing()) &&
           VerifyOffset(verifier, VT_MULTIPLEXING) &&
           verifier.VerifyString(multiplexing()) &&
           VerifyOffset(verifier, VT_MULTIPLE_ACCESS) &&
           verifier.VerifyString(multiple_access()) &&
           VerifyOffset(verifier, VT_SPREADING) &&
           verifier.VerifyString(spreading()) &&
           VerifyField<double>(verifier, VT_CHANNEL_BW) &&
           VerifyField<uint64_t>(verifier, VT_CHANNEL) &&
           VerifyOffset(verifier, VT_CLASS_VARIANT) &&
           verifier.VerifyString(class_variant()) &&
           verifier.EndTable();
  }
  signal_detailT *UnPack(const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(signal_detailT *_o, const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static flatbuffers::Offset<signal_detail> Pack(flatbuffers::FlatBufferBuilder &_fbb, const signal_detailT* _o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct signal_detailBuilder {
  typedef signal_detail Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_type(flatbuffers::Offset<flatbuffers::String> type) {
    fbb_.AddOffset(signal_detail::VT_TYPE, type);
  }
  void add_mod_class(flatbuffers::Offset<flatbuffers::String> mod_class) {
    fbb_.AddOffset(signal_detail::VT_MOD_CLASS, mod_class);
  }
  void add_standard(flatbuffers::Offset<flatbuffers::String> standard) {
    fbb_.AddOffset(signal_detail::VT_STANDARD, standard);
  }
  void add_carrier_variant(flatbuffers::Offset<flatbuffers::String> carrier_variant) {
    fbb_.AddOffset(signal_detail::VT_CARRIER_VARIANT, carrier_variant);
  }
  void add_symbol_variant(flatbuffers::Offset<flatbuffers::String> symbol_variant) {
    fbb_.AddOffset(signal_detail::VT_SYMBOL_VARIANT, symbol_variant);
  }
  void add_order(uint64_t order) {
    fbb_.AddElement<uint64_t>(signal_detail::VT_ORDER, order);
  }
  void add_duplexing(flatbuffers::Offset<flatbuffers::String> duplexing) {
    fbb_.AddOffset(signal_detail::VT_DUPLEXING, duplexing);
  }
  void add_multiplexing(flatbuffers::Offset<flatbuffers::String> multiplexing) {
    fbb_.AddOffset(signal_detail::VT_MULTIPLEXING, multiplexing);
  }
  void add_multiple_access(flatbuffers::Offset<flatbuffers::String> multiple_access) {
    fbb_.AddOffset(signal_detail::VT_MULTIPLE_ACCESS, multiple_access);
  }
  void add_spreading(flatbuffers::Offset<flatbuffers::String> spreading) {
    fbb_.AddOffset(signal_detail::VT_SPREADING, spreading);
  }
  void add_channel_bw(double channel_bw) {
    fbb_.AddElement<double>(signal_detail::VT_CHANNEL_BW, channel_bw);
  }
  void add_channel(uint64_t channel) {
    fbb_.AddElement<uint64_t>(signal_detail::VT_CHANNEL, channel);
  }
  void add_class_variant(flatbuffers::Offset<flatbuffers::String> class_variant) {
    fbb_.AddOffset(signal_detail::VT_CLASS_VARIANT, class_variant);
  }
  explicit signal_detailBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<signal_detail> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<signal_detail>(end);
    return o;
  }
};

inline flatbuffers::Offset<signal_detail> Createsignal_detail(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> type = 0,
    flatbuffers::Offset<flatbuffers::String> mod_class = 0,
    flatbuffers::Offset<flatbuffers::String> standard = 0,
    flatbuffers::Offset<flatbuffers::String> carrier_variant = 0,
    flatbuffers::Offset<flatbuffers::String> symbol_variant = 0,
    flatbuffers::Optional<uint64_t> order = flatbuffers::nullopt,
    flatbuffers::Offset<flatbuffers::String> duplexing = 0,
    flatbuffers::Offset<flatbuffers::String> multiplexing = 0,
    flatbuffers::Offset<flatbuffers::String> multiple_access = 0,
    flatbuffers::Offset<flatbuffers::String> spreading = 0,
    flatbuffers::Optional<double> channel_bw = flatbuffers::nullopt,
    flatbuffers::Optional<uint64_t> channel = flatbuffers::nullopt,
    flatbuffers::Offset<flatbuffers::String> class_variant = 0) {
  signal_detailBuilder builder_(_fbb);
  if(channel) { builder_.add_channel(*channel); }
  if(channel_bw) { builder_.add_channel_bw(*channel_bw); }
  if(order) { builder_.add_order(*order); }
  builder_.add_class_variant(class_variant);
  builder_.add_spreading(spreading);
  builder_.add_multiple_access(multiple_access);
  builder_.add_multiplexing(multiplexing);
  builder_.add_duplexing(duplexing);
  builder_.add_symbol_variant(symbol_variant);
  builder_.add_carrier_variant(carrier_variant);
  builder_.add_standard(standard);
  builder_.add_mod_class(mod_class);
  builder_.add_type(type);
  return builder_.Finish();
}

inline flatbuffers::Offset<signal_detail> Createsignal_detailDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *type = nullptr,
    const char *mod_class = nullptr,
    const char *standard = nullptr,
    const char *carrier_variant = nullptr,
    const char *symbol_variant = nullptr,
    flatbuffers::Optional<uint64_t> order = flatbuffers::nullopt,
    const char *duplexing = nullptr,
    const char *multiplexing = nullptr,
    const char *multiple_access = nullptr,
    const char *spreading = nullptr,
    flatbuffers::Optional<double> channel_bw = flatbuffers::nullopt,
    flatbuffers::Optional<uint64_t> channel = flatbuffers::nullopt,
    const char *class_variant = nullptr) {
  auto type__ = type ? _fbb.CreateString(type) : 0;
  auto mod_class__ = mod_class ? _fbb.CreateString(mod_class) : 0;
  auto standard__ = standard ? _fbb.CreateString(standard) : 0;
  auto carrier_variant__ = carrier_variant ? _fbb.CreateString(carrier_variant) : 0;
  auto symbol_variant__ = symbol_variant ? _fbb.CreateString(symbol_variant) : 0;
  auto duplexing__ = duplexing ? _fbb.CreateString(duplexing) : 0;
  auto multiplexing__ = multiplexing ? _fbb.CreateString(multiplexing) : 0;
  auto multiple_access__ = multiple_access ? _fbb.CreateString(multiple_access) : 0;
  auto spreading__ = spreading ? _fbb.CreateString(spreading) : 0;
  auto class_variant__ = class_variant ? _fbb.CreateString(class_variant) : 0;
  return sigmf::signal::Createsignal_detail(
      _fbb,
      type__,
      mod_class__,
      standard__,
      carrier_variant__,
      symbol_variant__,
      order,
      duplexing__,
      multiplexing__,
      multiple_access__,
      spreading__,
      channel_bw,
      channel,
      class_variant__);
}

flatbuffers::Offset<signal_detail> Createsignal_detail(flatbuffers::FlatBufferBuilder &_fbb, const signal_detailT *_o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);

struct signal_emitterT : public flatbuffers::NativeTable {
  typedef signal_emitter TableType;
  flatbuffers::Optional<uint64_t> seid = flatbuffers::nullopt;
  std::string manufacturer{};
  flatbuffers::Optional<double> power_tx = flatbuffers::nullopt;
  flatbuffers::Optional<double> power_eirp = flatbuffers::nullopt;
  std::shared_ptr<sigmf::core::geojson_pointT> geolocation{};
};

struct signal_emitter FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef signal_emitterT NativeTableType;
  typedef signal_emitterBuilder Builder;
  static const flatbuffers::TypeTable *MiniReflectTypeTable() {
    return signal_emitterTypeTable();
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_SEID = 4,
    VT_MANUFACTURER = 6,
    VT_POWER_TX = 8,
    VT_POWER_EIRP = 10,
    VT_GEOLOCATION = 12
  };
  flatbuffers::Optional<uint64_t> seid() const {
    return GetOptional<uint64_t, uint64_t>(VT_SEID);
  }
  const flatbuffers::String *manufacturer() const {
    return GetPointer<const flatbuffers::String *>(VT_MANUFACTURER);
  }
  flatbuffers::Optional<double> power_tx() const {
    return GetOptional<double, double>(VT_POWER_TX);
  }
  flatbuffers::Optional<double> power_eirp() const {
    return GetOptional<double, double>(VT_POWER_EIRP);
  }
  const sigmf::core::geojson_point *geolocation() const {
    return GetPointer<const sigmf::core::geojson_point *>(VT_GEOLOCATION);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint64_t>(verifier, VT_SEID) &&
           VerifyOffset(verifier, VT_MANUFACTURER) &&
           verifier.VerifyString(manufacturer()) &&
           VerifyField<double>(verifier, VT_POWER_TX) &&
           VerifyField<double>(verifier, VT_POWER_EIRP) &&
           VerifyOffset(verifier, VT_GEOLOCATION) &&
           verifier.VerifyTable(geolocation()) &&
           verifier.EndTable();
  }
  signal_emitterT *UnPack(const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(signal_emitterT *_o, const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static flatbuffers::Offset<signal_emitter> Pack(flatbuffers::FlatBufferBuilder &_fbb, const signal_emitterT* _o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct signal_emitterBuilder {
  typedef signal_emitter Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_seid(uint64_t seid) {
    fbb_.AddElement<uint64_t>(signal_emitter::VT_SEID, seid);
  }
  void add_manufacturer(flatbuffers::Offset<flatbuffers::String> manufacturer) {
    fbb_.AddOffset(signal_emitter::VT_MANUFACTURER, manufacturer);
  }
  void add_power_tx(double power_tx) {
    fbb_.AddElement<double>(signal_emitter::VT_POWER_TX, power_tx);
  }
  void add_power_eirp(double power_eirp) {
    fbb_.AddElement<double>(signal_emitter::VT_POWER_EIRP, power_eirp);
  }
  void add_geolocation(flatbuffers::Offset<sigmf::core::geojson_point> geolocation) {
    fbb_.AddOffset(signal_emitter::VT_GEOLOCATION, geolocation);
  }
  explicit signal_emitterBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<signal_emitter> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<signal_emitter>(end);
    return o;
  }
};

inline flatbuffers::Offset<signal_emitter> Createsignal_emitter(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Optional<uint64_t> seid = flatbuffers::nullopt,
    flatbuffers::Offset<flatbuffers::String> manufacturer = 0,
    flatbuffers::Optional<double> power_tx = flatbuffers::nullopt,
    flatbuffers::Optional<double> power_eirp = flatbuffers::nullopt,
    flatbuffers::Offset<sigmf::core::geojson_point> geolocation = 0) {
  signal_emitterBuilder builder_(_fbb);
  if(power_eirp) { builder_.add_power_eirp(*power_eirp); }
  if(power_tx) { builder_.add_power_tx(*power_tx); }
  if(seid) { builder_.add_seid(*seid); }
  builder_.add_geolocation(geolocation);
  builder_.add_manufacturer(manufacturer);
  return builder_.Finish();
}

inline flatbuffers::Offset<signal_emitter> Createsignal_emitterDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Optional<uint64_t> seid = flatbuffers::nullopt,
    const char *manufacturer = nullptr,
    flatbuffers::Optional<double> power_tx = flatbuffers::nullopt,
    flatbuffers::Optional<double> power_eirp = flatbuffers::nullopt,
    flatbuffers::Offset<sigmf::core::geojson_point> geolocation = 0) {
  auto manufacturer__ = manufacturer ? _fbb.CreateString(manufacturer) : 0;
  return sigmf::signal::Createsignal_emitter(
      _fbb,
      seid,
      manufacturer__,
      power_tx,
      power_eirp,
      geolocation);
}

flatbuffers::Offset<signal_emitter> Createsignal_emitter(flatbuffers::FlatBufferBuilder &_fbb, const signal_emitterT *_o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);

struct AnnotationT : public flatbuffers::NativeTable {
  typedef Annotation TableType;
  std::shared_ptr<sigmf::signal::signal_detailT> detail{};
  std::shared_ptr<sigmf::signal::signal_emitterT> emitter{};
};

struct Annotation FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef AnnotationT NativeTableType;
  typedef AnnotationBuilder Builder;
  static const flatbuffers::TypeTable *MiniReflectTypeTable() {
    return AnnotationTypeTable();
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_DETAIL = 4,
    VT_EMITTER = 6
  };
  const sigmf::signal::signal_detail *detail() const {
    return GetPointer<const sigmf::signal::signal_detail *>(VT_DETAIL);
  }
  const sigmf::signal::signal_emitter *emitter() const {
    return GetPointer<const sigmf::signal::signal_emitter *>(VT_EMITTER);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_DETAIL) &&
           verifier.VerifyTable(detail()) &&
           VerifyOffset(verifier, VT_EMITTER) &&
           verifier.VerifyTable(emitter()) &&
           verifier.EndTable();
  }
  AnnotationT *UnPack(const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(AnnotationT *_o, const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static flatbuffers::Offset<Annotation> Pack(flatbuffers::FlatBufferBuilder &_fbb, const AnnotationT* _o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct AnnotationBuilder {
  typedef Annotation Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_detail(flatbuffers::Offset<sigmf::signal::signal_detail> detail) {
    fbb_.AddOffset(Annotation::VT_DETAIL, detail);
  }
  void add_emitter(flatbuffers::Offset<sigmf::signal::signal_emitter> emitter) {
    fbb_.AddOffset(Annotation::VT_EMITTER, emitter);
  }
  explicit AnnotationBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<Annotation> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Annotation>(end);
    return o;
  }
};

inline flatbuffers::Offset<Annotation> CreateAnnotation(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<sigmf::signal::signal_detail> detail = 0,
    flatbuffers::Offset<sigmf::signal::signal_emitter> emitter = 0) {
  AnnotationBuilder builder_(_fbb);
  builder_.add_emitter(emitter);
  builder_.add_detail(detail);
  return builder_.Finish();
}

flatbuffers::Offset<Annotation> CreateAnnotation(flatbuffers::FlatBufferBuilder &_fbb, const AnnotationT *_o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);

struct DescrT : public flatbuffers::NativeTable {
  typedef Descr TableType;
  std::shared_ptr<sigmf::signal::GlobalT> global{};
  std::shared_ptr<sigmf::signal::AnnotationT> annotation{};
  std::shared_ptr<sigmf::signal::CaptureT> capture{};
};

struct Descr FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef DescrT NativeTableType;
  typedef DescrBuilder Builder;
  static const flatbuffers::TypeTable *MiniReflectTypeTable() {
    return DescrTypeTable();
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_GLOBAL = 4,
    VT_ANNOTATION = 6,
    VT_CAPTURE = 8
  };
  const sigmf::signal::Global *global() const {
    return GetPointer<const sigmf::signal::Global *>(VT_GLOBAL);
  }
  const sigmf::signal::Annotation *annotation() const {
    return GetPointer<const sigmf::signal::Annotation *>(VT_ANNOTATION);
  }
  const sigmf::signal::Capture *capture() const {
    return GetPointer<const sigmf::signal::Capture *>(VT_CAPTURE);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_GLOBAL) &&
           verifier.VerifyTable(global()) &&
           VerifyOffset(verifier, VT_ANNOTATION) &&
           verifier.VerifyTable(annotation()) &&
           VerifyOffset(verifier, VT_CAPTURE) &&
           verifier.VerifyTable(capture()) &&
           verifier.EndTable();
  }
  DescrT *UnPack(const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(DescrT *_o, const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static flatbuffers::Offset<Descr> Pack(flatbuffers::FlatBufferBuilder &_fbb, const DescrT* _o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct DescrBuilder {
  typedef Descr Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_global(flatbuffers::Offset<sigmf::signal::Global> global) {
    fbb_.AddOffset(Descr::VT_GLOBAL, global);
  }
  void add_annotation(flatbuffers::Offset<sigmf::signal::Annotation> annotation) {
    fbb_.AddOffset(Descr::VT_ANNOTATION, annotation);
  }
  void add_capture(flatbuffers::Offset<sigmf::signal::Capture> capture) {
    fbb_.AddOffset(Descr::VT_CAPTURE, capture);
  }
  explicit DescrBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<Descr> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Descr>(end);
    return o;
  }
};

inline flatbuffers::Offset<Descr> CreateDescr(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<sigmf::signal::Global> global = 0,
    flatbuffers::Offset<sigmf::signal::Annotation> annotation = 0,
    flatbuffers::Offset<sigmf::signal::Capture> capture = 0) {
  DescrBuilder builder_(_fbb);
  builder_.add_capture(capture);
  builder_.add_annotation(annotation);
  builder_.add_global(global);
  return builder_.Finish();
}

flatbuffers::Offset<Descr> CreateDescr(flatbuffers::FlatBufferBuilder &_fbb, const DescrT *_o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);

inline GlobalT *Global::UnPack(const flatbuffers::resolver_function_t *_resolver) const {
  auto _o = std::unique_ptr<GlobalT>(new GlobalT());
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void Global::UnPackTo(GlobalT *_o, const flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
}

inline flatbuffers::Offset<Global> Global::Pack(flatbuffers::FlatBufferBuilder &_fbb, const GlobalT* _o, const flatbuffers::rehasher_function_t *_rehasher) {
  return CreateGlobal(_fbb, _o, _rehasher);
}

inline flatbuffers::Offset<Global> CreateGlobal(flatbuffers::FlatBufferBuilder &_fbb, const GlobalT *_o, const flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { flatbuffers::FlatBufferBuilder *__fbb; const GlobalT* __o; const flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  return sigmf::signal::CreateGlobal(
      _fbb);
}

inline CaptureT *Capture::UnPack(const flatbuffers::resolver_function_t *_resolver) const {
  auto _o = std::unique_ptr<CaptureT>(new CaptureT());
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void Capture::UnPackTo(CaptureT *_o, const flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
}

inline flatbuffers::Offset<Capture> Capture::Pack(flatbuffers::FlatBufferBuilder &_fbb, const CaptureT* _o, const flatbuffers::rehasher_function_t *_rehasher) {
  return CreateCapture(_fbb, _o, _rehasher);
}

inline flatbuffers::Offset<Capture> CreateCapture(flatbuffers::FlatBufferBuilder &_fbb, const CaptureT *_o, const flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { flatbuffers::FlatBufferBuilder *__fbb; const CaptureT* __o; const flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  return sigmf::signal::CreateCapture(
      _fbb);
}

inline signal_detailT *signal_detail::UnPack(const flatbuffers::resolver_function_t *_resolver) const {
  auto _o = std::unique_ptr<signal_detailT>(new signal_detailT());
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void signal_detail::UnPackTo(signal_detailT *_o, const flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = type(); if (_e) _o->type = _e->str(); }
  { auto _e = mod_class(); if (_e) _o->mod_class = _e->str(); }
  { auto _e = standard(); if (_e) _o->standard = _e->str(); }
  { auto _e = carrier_variant(); if (_e) _o->carrier_variant = _e->str(); }
  { auto _e = symbol_variant(); if (_e) _o->symbol_variant = _e->str(); }
  { auto _e = order(); _o->order = _e; }
  { auto _e = duplexing(); if (_e) _o->duplexing = _e->str(); }
  { auto _e = multiplexing(); if (_e) _o->multiplexing = _e->str(); }
  { auto _e = multiple_access(); if (_e) _o->multiple_access = _e->str(); }
  { auto _e = spreading(); if (_e) _o->spreading = _e->str(); }
  { auto _e = channel_bw(); _o->channel_bw = _e; }
  { auto _e = channel(); _o->channel = _e; }
  { auto _e = class_variant(); if (_e) _o->class_variant = _e->str(); }
}

inline flatbuffers::Offset<signal_detail> signal_detail::Pack(flatbuffers::FlatBufferBuilder &_fbb, const signal_detailT* _o, const flatbuffers::rehasher_function_t *_rehasher) {
  return Createsignal_detail(_fbb, _o, _rehasher);
}

inline flatbuffers::Offset<signal_detail> Createsignal_detail(flatbuffers::FlatBufferBuilder &_fbb, const signal_detailT *_o, const flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { flatbuffers::FlatBufferBuilder *__fbb; const signal_detailT* __o; const flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _type = _o->type.empty() ? 0 : _fbb.CreateString(_o->type);
  auto _mod_class = _o->mod_class.empty() ? 0 : _fbb.CreateString(_o->mod_class);
  auto _standard = _o->standard.empty() ? 0 : _fbb.CreateString(_o->standard);
  auto _carrier_variant = _o->carrier_variant.empty() ? 0 : _fbb.CreateString(_o->carrier_variant);
  auto _symbol_variant = _o->symbol_variant.empty() ? 0 : _fbb.CreateString(_o->symbol_variant);
  auto _order = _o->order;
  auto _duplexing = _o->duplexing.empty() ? 0 : _fbb.CreateString(_o->duplexing);
  auto _multiplexing = _o->multiplexing.empty() ? 0 : _fbb.CreateString(_o->multiplexing);
  auto _multiple_access = _o->multiple_access.empty() ? 0 : _fbb.CreateString(_o->multiple_access);
  auto _spreading = _o->spreading.empty() ? 0 : _fbb.CreateString(_o->spreading);
  auto _channel_bw = _o->channel_bw;
  auto _channel = _o->channel;
  auto _class_variant = _o->class_variant.empty() ? 0 : _fbb.CreateString(_o->class_variant);
  return sigmf::signal::Createsignal_detail(
      _fbb,
      _type,
      _mod_class,
      _standard,
      _carrier_variant,
      _symbol_variant,
      _order,
      _duplexing,
      _multiplexing,
      _multiple_access,
      _spreading,
      _channel_bw,
      _channel,
      _class_variant);
}

inline signal_emitterT *signal_emitter::UnPack(const flatbuffers::resolver_function_t *_resolver) const {
  auto _o = std::unique_ptr<signal_emitterT>(new signal_emitterT());
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void signal_emitter::UnPackTo(signal_emitterT *_o, const flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = seid(); _o->seid = _e; }
  { auto _e = manufacturer(); if (_e) _o->manufacturer = _e->str(); }
  { auto _e = power_tx(); _o->power_tx = _e; }
  { auto _e = power_eirp(); _o->power_eirp = _e; }
  { auto _e = geolocation(); if (_e) _o->geolocation = std::shared_ptr<sigmf::core::geojson_pointT>(_e->UnPack(_resolver)); }
}

inline flatbuffers::Offset<signal_emitter> signal_emitter::Pack(flatbuffers::FlatBufferBuilder &_fbb, const signal_emitterT* _o, const flatbuffers::rehasher_function_t *_rehasher) {
  return Createsignal_emitter(_fbb, _o, _rehasher);
}

inline flatbuffers::Offset<signal_emitter> Createsignal_emitter(flatbuffers::FlatBufferBuilder &_fbb, const signal_emitterT *_o, const flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { flatbuffers::FlatBufferBuilder *__fbb; const signal_emitterT* __o; const flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _seid = _o->seid;
  auto _manufacturer = _o->manufacturer.empty() ? 0 : _fbb.CreateString(_o->manufacturer);
  auto _power_tx = _o->power_tx;
  auto _power_eirp = _o->power_eirp;
  auto _geolocation = _o->geolocation ? Creategeojson_point(_fbb, _o->geolocation.get(), _rehasher) : 0;
  return sigmf::signal::Createsignal_emitter(
      _fbb,
      _seid,
      _manufacturer,
      _power_tx,
      _power_eirp,
      _geolocation);
}

inline AnnotationT *Annotation::UnPack(const flatbuffers::resolver_function_t *_resolver) const {
  auto _o = std::unique_ptr<AnnotationT>(new AnnotationT());
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void Annotation::UnPackTo(AnnotationT *_o, const flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = detail(); if (_e) _o->detail = std::shared_ptr<sigmf::signal::signal_detailT>(_e->UnPack(_resolver)); }
  { auto _e = emitter(); if (_e) _o->emitter = std::shared_ptr<sigmf::signal::signal_emitterT>(_e->UnPack(_resolver)); }
}

inline flatbuffers::Offset<Annotation> Annotation::Pack(flatbuffers::FlatBufferBuilder &_fbb, const AnnotationT* _o, const flatbuffers::rehasher_function_t *_rehasher) {
  return CreateAnnotation(_fbb, _o, _rehasher);
}

inline flatbuffers::Offset<Annotation> CreateAnnotation(flatbuffers::FlatBufferBuilder &_fbb, const AnnotationT *_o, const flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { flatbuffers::FlatBufferBuilder *__fbb; const AnnotationT* __o; const flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _detail = _o->detail ? Createsignal_detail(_fbb, _o->detail.get(), _rehasher) : 0;
  auto _emitter = _o->emitter ? Createsignal_emitter(_fbb, _o->emitter.get(), _rehasher) : 0;
  return sigmf::signal::CreateAnnotation(
      _fbb,
      _detail,
      _emitter);
}

inline DescrT *Descr::UnPack(const flatbuffers::resolver_function_t *_resolver) const {
  auto _o = std::unique_ptr<DescrT>(new DescrT());
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void Descr::UnPackTo(DescrT *_o, const flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = global(); if (_e) _o->global = std::shared_ptr<sigmf::signal::GlobalT>(_e->UnPack(_resolver)); }
  { auto _e = annotation(); if (_e) _o->annotation = std::shared_ptr<sigmf::signal::AnnotationT>(_e->UnPack(_resolver)); }
  { auto _e = capture(); if (_e) _o->capture = std::shared_ptr<sigmf::signal::CaptureT>(_e->UnPack(_resolver)); }
}

inline flatbuffers::Offset<Descr> Descr::Pack(flatbuffers::FlatBufferBuilder &_fbb, const DescrT* _o, const flatbuffers::rehasher_function_t *_rehasher) {
  return CreateDescr(_fbb, _o, _rehasher);
}

inline flatbuffers::Offset<Descr> CreateDescr(flatbuffers::FlatBufferBuilder &_fbb, const DescrT *_o, const flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { flatbuffers::FlatBufferBuilder *__fbb; const DescrT* __o; const flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _global = _o->global ? CreateGlobal(_fbb, _o->global.get(), _rehasher) : 0;
  auto _annotation = _o->annotation ? CreateAnnotation(_fbb, _o->annotation.get(), _rehasher) : 0;
  auto _capture = _o->capture ? CreateCapture(_fbb, _o->capture.get(), _rehasher) : 0;
  return sigmf::signal::CreateDescr(
      _fbb,
      _global,
      _annotation,
      _capture);
}

inline const flatbuffers::TypeTable *GlobalTypeTable() {
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_TABLE, 0, nullptr, nullptr, nullptr, nullptr, nullptr
  };
  return &tt;
}

inline const flatbuffers::TypeTable *CaptureTypeTable() {
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_TABLE, 0, nullptr, nullptr, nullptr, nullptr, nullptr
  };
  return &tt;
}

inline const flatbuffers::TypeTable *signal_detailTypeTable() {
  static const flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_STRING, 0, -1 },
    { flatbuffers::ET_STRING, 0, -1 },
    { flatbuffers::ET_STRING, 0, -1 },
    { flatbuffers::ET_STRING, 0, -1 },
    { flatbuffers::ET_STRING, 0, -1 },
    { flatbuffers::ET_ULONG, 0, -1 },
    { flatbuffers::ET_STRING, 0, -1 },
    { flatbuffers::ET_STRING, 0, -1 },
    { flatbuffers::ET_STRING, 0, -1 },
    { flatbuffers::ET_STRING, 0, -1 },
    { flatbuffers::ET_DOUBLE, 0, -1 },
    { flatbuffers::ET_ULONG, 0, -1 },
    { flatbuffers::ET_STRING, 0, -1 }
  };
  static const char * const names[] = {
    "type",
    "mod_class",
    "standard",
    "carrier_variant",
    "symbol_variant",
    "order",
    "duplexing",
    "multiplexing",
    "multiple_access",
    "spreading",
    "channel_bw",
    "channel",
    "class_variant"
  };
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_TABLE, 13, type_codes, nullptr, nullptr, nullptr, names
  };
  return &tt;
}

inline const flatbuffers::TypeTable *signal_emitterTypeTable() {
  static const flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_ULONG, 0, -1 },
    { flatbuffers::ET_STRING, 0, -1 },
    { flatbuffers::ET_DOUBLE, 0, -1 },
    { flatbuffers::ET_DOUBLE, 0, -1 },
    { flatbuffers::ET_SEQUENCE, 0, 0 }
  };
  static const flatbuffers::TypeFunction type_refs[] = {
    sigmf::core::geojson_pointTypeTable
  };
  static const char * const names[] = {
    "seid",
    "manufacturer",
    "power_tx",
    "power_eirp",
    "geolocation"
  };
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_TABLE, 5, type_codes, type_refs, nullptr, nullptr, names
  };
  return &tt;
}

inline const flatbuffers::TypeTable *AnnotationTypeTable() {
  static const flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_SEQUENCE, 0, 0 },
    { flatbuffers::ET_SEQUENCE, 0, 1 }
  };
  static const flatbuffers::TypeFunction type_refs[] = {
    sigmf::signal::signal_detailTypeTable,
    sigmf::signal::signal_emitterTypeTable
  };
  static const char * const names[] = {
    "detail",
    "emitter"
  };
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_TABLE, 2, type_codes, type_refs, nullptr, nullptr, names
  };
  return &tt;
}

inline const flatbuffers::TypeTable *DescrTypeTable() {
  static const flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_SEQUENCE, 0, 0 },
    { flatbuffers::ET_SEQUENCE, 0, 1 },
    { flatbuffers::ET_SEQUENCE, 0, 2 }
  };
  static const flatbuffers::TypeFunction type_refs[] = {
    sigmf::signal::GlobalTypeTable,
    sigmf::signal::AnnotationTypeTable,
    sigmf::signal::CaptureTypeTable
  };
  static const char * const names[] = {
    "global",
    "annotation",
    "capture"
  };
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_TABLE, 3, type_codes, type_refs, nullptr, nullptr, names
  };
  return &tt;
}

}  // namespace signal
}  // namespace sigmf

#endif  // FLATBUFFERS_GENERATED_SIGMFSIGNAL_SIGMF_SIGNAL_H_
