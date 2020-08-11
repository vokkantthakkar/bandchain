// TODO: Replace this mock by the real

type request_t =
  | Request(RequestSub.t)
  | RequestMini(RequestSub.Mini.t);

type request_packet_t = {
  clientID: string,
  oracleScriptID: int,
  calldata: JsBuffer.t,
  askCount: int,
  minCount: int,
};

type response_packet_t = {
  clientID: string,
  requestID: int,
  ansCount: int,
  requestTime: int,
  resolveTime: int,
  resolveStatus: int,
  result: JsBuffer.t,
};

type iavl_merkle_path_t = {
  isDataOnRight: bool,
  subtreeHeight: int,
  subtreeSize: int,
  subtreeVersion: int,
  siblingHash: JsBuffer.t,
};

type oracle_data_proof_t = {
  requestPacket: request_packet_t,
  responsePacket: response_packet_t,
  version: int,
  iavl_merkle_paths: list(iavl_merkle_path_t),
};

type multi_store_proof_t = {
  accToGovStoresMerkleHash: JsBuffer.t,
  mainAndMintStoresMerkleHash: JsBuffer.t,
  oracleIAVLStateHash: JsBuffer.t,
  paramsStoresMerkleHash: JsBuffer.t,
  slashingToUpgradeStoresMerkleHash: JsBuffer.t,
};

type block_header_merkle_parts_t = {
  versionAndChainIdHash: JsBuffer.t,
  timeHash: JsBuffer.t,
  lastBlockIDAndOther: JsBuffer.t,
  nextValidatorHashAndConsensusHash: JsBuffer.t,
  lastResultsHash: JsBuffer.t,
  evidenceAndProposerHash: JsBuffer.t,
};

type tm_signature_t = {
  r: JsBuffer.t,
  s: JsBuffer.t,
  v: int,
  signedPrefixSuffix: JsBuffer.t,
  signedDataSuffix: JsBuffer.t,
};

type block_relay_proof_t = {
  multiStoreProof: multi_store_proof_t,
  blockHeaderMerkleParts: block_header_merkle_parts_t,
  signatures: list(tm_signature_t),
};

type proof_t = {
  blockHeight: int,
  oracleDataProof: oracle_data_proof_t,
  blockRelayProof: block_relay_proof_t,
};

let decodeRequestPacket = (json: Js.Json.t) => {
  JsonUtils.Decode.{
    clientID: json |> field("client_id", string),
    oracleScriptID: json |> field("oracle_script_id", intstr),
    calldata: json |> field("calldata", string) |> JsBuffer.fromBase64,
    askCount: json |> field("ask_count", intstr),
    minCount: json |> field("min_count", intstr),
  };
};

let decodeResponsePacket = json => {
  JsonUtils.Decode.{
    clientID: json |> field("client_id", string),
    requestID: json |> field("request_id", intstr),
    ansCount: json |> field("ans_count", intstr),
    requestTime: json |> field("request_time", intstr),
    resolveTime: json |> field("resolve_time", intstr),
    resolveStatus: json |> field("resolve_status", int),
    result: json |> field("result", string) |> JsBuffer.fromBase64,
  };
};

let decodeIAVLMerklePath = json => {
  JsonUtils.Decode.{
    isDataOnRight: json |> field("isDataOnRight", bool),
    subtreeHeight: json |> field("subtreeHeight", int),
    subtreeSize: json |> field("subtreeSize", intstr),
    subtreeVersion: json |> field("subtreeVersion", intstr),
    siblingHash: json |> field("siblingHash", string) |> JsBuffer.fromHex,
  };
};

let decodeOracleDataProof = json => {
  JsonUtils.Decode.{
    requestPacket: json |> field("requestPacket", decodeRequestPacket),
    responsePacket: json |> field("responsePacket", decodeResponsePacket),
    version: json |> field("version", intstr),
    iavl_merkle_paths: json |> field("merklePaths", list(decodeIAVLMerklePath)),
  };
};

let decodeMultiStoreProof = json => {
  JsonUtils.Decode.{
    accToGovStoresMerkleHash:
      json |> field("accToGovStoresMerkleHash", string) |> JsBuffer.fromHex,
    mainAndMintStoresMerkleHash:
      json |> field("mainAndMintStoresMerkleHash", string) |> JsBuffer.fromHex,
    oracleIAVLStateHash: json |> field("oracleIAVLStateHash", string) |> JsBuffer.fromHex,
    paramsStoresMerkleHash: json |> field("paramsStoresMerkleHash", string) |> JsBuffer.fromHex,
    slashingToUpgradeStoresMerkleHash:
      json |> field("slashingToUpgradeStoresMerkleHash", string) |> JsBuffer.fromHex,
  };
};

let decodeBlockHeaderMerkleParts = json => {
  JsonUtils.Decode.{
    versionAndChainIdHash: json |> field("versionAndChainIdHash", string) |> JsBuffer.fromHex,
    timeHash: json |> field("timeHash", string) |> JsBuffer.fromHex,
    lastBlockIDAndOther: json |> field("lastBlockIDAndOther", string) |> JsBuffer.fromHex,
    nextValidatorHashAndConsensusHash:
      json |> field("nextValidatorHashAndConsensusHash", string) |> JsBuffer.fromHex,
    lastResultsHash: json |> field("lastResultsHash", string) |> JsBuffer.fromHex,
    evidenceAndProposerHash: json |> field("evidenceAndProposerHash", string) |> JsBuffer.fromHex,
  };
};

let decodeTMSignature = json => {
  JsonUtils.Decode.{
    r: json |> field("r", string) |> JsBuffer.fromHex,
    s: json |> field("s", string) |> JsBuffer.fromHex,
    v: json |> field("v", int),
    signedPrefixSuffix: json |> field("signedPrefixSuffix", string) |> JsBuffer.fromHex,
    signedDataSuffix: json |> field("signedDataSuffix", string) |> JsBuffer.fromHex,
  };
};

let decodeBlockRelayProof = json => {
  JsonUtils.Decode.{
    multiStoreProof: json |> field("multiStoreProof", decodeMultiStoreProof),
    blockHeaderMerkleParts: json |> field("blockHeaderMerkleParts", decodeBlockHeaderMerkleParts),
    signatures: json |> field("signatures", list(decodeTMSignature)),
  };
};

let decodeProof = json =>
  JsonUtils.Decode.{
    blockHeight: json |> field("blockHeight", intstr),
    oracleDataProof: json |> field("oracleDataProof", decodeOracleDataProof),
    blockRelayProof: json |> field("blockRelayProof", decodeBlockRelayProof),
  };

let obi_encode_int = (i, n) =>
  Obi.encode(
    "{x: " ++ n ++ "}/{_:u64}",
    "input",
    [|{fieldName: "x", fieldValue: i |> string_of_int}|],
  )
  |> Belt_Option.getExn;

type variant_of_proof_t =
  | RequestPacket(request_packet_t)
  | ResponsePacket(response_packet_t)
  | IAVLMerklePath(iavl_merkle_path_t)
  | IAVLMerklePaths(list(iavl_merkle_path_t))
  | MultiStoreProof(multi_store_proof_t)
  | BlockHeaderMerkleParts(block_header_merkle_parts_t)
  | Signature(tm_signature_t)
  | Signatures(list(tm_signature_t))
  | OracleDataProof(oracle_data_proof_t)
  | BlockRelayProof(block_relay_proof_t)
  | Proof(proof_t);

let rec encode =
  fun
  | RequestPacket({clientID, oracleScriptID, calldata, askCount, minCount}) => {
      Obi.encode(
        {j|{clientID: string, oracleScriptID: u64, calldata: bytes, askCount: u64, minCount: u64}/{_:u64}|j},
        "input",
        [|
          {fieldName: "clientID", fieldValue: clientID},
          {fieldName: "oracleScriptID", fieldValue: oracleScriptID |> string_of_int},
          {fieldName: "calldata", fieldValue: calldata |> JsBuffer.toHex(~with0x=true)},
          {fieldName: "askCount", fieldValue: askCount |> string_of_int},
          {fieldName: "minCount", fieldValue: minCount |> string_of_int},
        |],
      );
    }
  | ResponsePacket({
      clientID,
      requestID,
      ansCount,
      requestTime,
      resolveTime,
      resolveStatus,
      result,
    }) => {
      Obi.encode(
        {j|{clientID: string, requestID: u64, ansCount: u64, requestTime: u64, resolveTime: u64, resolveStatus: u32, result: bytes}/{_:u64}|j},
        "input",
        [|
          {fieldName: "clientID", fieldValue: clientID},
          {fieldName: "requestID", fieldValue: requestID |> string_of_int},
          {fieldName: "ansCount", fieldValue: ansCount |> string_of_int},
          {fieldName: "requestTime", fieldValue: requestTime |> string_of_int},
          {fieldName: "resolveTime", fieldValue: resolveTime |> string_of_int},
          {fieldName: "resolveStatus", fieldValue: resolveStatus |> string_of_int},
          {fieldName: "result", fieldValue: result |> JsBuffer.toHex(~with0x=true)},
        |],
      );
    }
  | IAVLMerklePath({isDataOnRight, subtreeHeight, subtreeSize, subtreeVersion, siblingHash}) => {
      Obi.encode(
        {j|{isDataOnRight: u8, subtreeHeight: u8, subtreeSize: u64, subtreeVersion: u64, siblingHash: bytes}/{_:u64}|j},
        "input",
        [|
          {fieldName: "isDataOnRight", fieldValue: isDataOnRight ? "1" : "0"},
          {fieldName: "subtreeHeight", fieldValue: subtreeHeight |> string_of_int},
          {fieldName: "subtreeSize", fieldValue: subtreeSize |> string_of_int},
          {fieldName: "subtreeVersion", fieldValue: subtreeVersion |> string_of_int},
          {fieldName: "siblingHash", fieldValue: siblingHash |> JsBuffer.toHex(~with0x=true)},
        |],
      );
    }
  | IAVLMerklePaths(list_of_iavl_merkle_paths) => {
      list_of_iavl_merkle_paths
      |> Belt_List.map(_, x => encode(IAVLMerklePath(x)))
      |> Belt_List.reduce(_, Some(JsBuffer.from([||])), (a, b) =>
           switch (a, b) {
           | (Some(acc), Some(elem)) => Some(JsBuffer.concat([|acc, elem|]))
           | _ => None
           }
         )
      |> Belt_Option.map(_, x =>
           JsBuffer.concat([|
             obi_encode_int(list_of_iavl_merkle_paths |> Belt_List.length, "u32"),
             x,
           |])
         );
    }
  | MultiStoreProof({
      accToGovStoresMerkleHash,
      mainAndMintStoresMerkleHash,
      oracleIAVLStateHash,
      paramsStoresMerkleHash,
      slashingToUpgradeStoresMerkleHash,
    }) => {
      Some(
        JsBuffer.concat([|
          accToGovStoresMerkleHash,
          mainAndMintStoresMerkleHash,
          oracleIAVLStateHash,
          paramsStoresMerkleHash,
          slashingToUpgradeStoresMerkleHash,
        |]),
      );
    }
  | BlockHeaderMerkleParts({
      versionAndChainIdHash,
      timeHash,
      lastBlockIDAndOther,
      nextValidatorHashAndConsensusHash,
      lastResultsHash,
      evidenceAndProposerHash,
    }) => {
      Some(
        JsBuffer.concat([|
          versionAndChainIdHash,
          timeHash,
          lastBlockIDAndOther,
          nextValidatorHashAndConsensusHash,
          lastResultsHash,
          evidenceAndProposerHash,
        |]),
      );
    }
  | Signature({r, s, v, signedPrefixSuffix, signedDataSuffix}) => {
      Obi.encode(
        {j|{r: bytes, s: bytes, v: u8, signedPrefixSuffix: bytes, signedDataSuffix: bytes}/{_:u64}|j},
        "input",
        [|
          {fieldName: "r", fieldValue: r |> JsBuffer.toHex(~with0x=true)},
          {fieldName: "s", fieldValue: s |> JsBuffer.toHex(~with0x=true)},
          {fieldName: "v", fieldValue: v |> string_of_int},
          {
            fieldName: "signedPrefixSuffix",
            fieldValue: signedPrefixSuffix |> JsBuffer.toHex(~with0x=true),
          },
          {
            fieldName: "signedDataSuffix",
            fieldValue: signedDataSuffix |> JsBuffer.toHex(~with0x=true),
          },
        |],
      );
    }
  | Signatures(list_of_tm_signatures) => {
      list_of_tm_signatures
      |> Belt_List.map(_, x => encode(Signature(x)))
      |> Belt_List.reduce(_, Some(JsBuffer.from([||])), (a, b) =>
           switch (a, b) {
           | (Some(acc), Some(elem)) => Some(JsBuffer.concat([|acc, elem|]))
           | _ => None
           }
         )
      |> Belt_Option.map(_, x =>
           JsBuffer.concat([|
             obi_encode_int(list_of_tm_signatures |> Belt_List.length, "u32"),
             x,
           |])
         );
    }
  | OracleDataProof({requestPacket, responsePacket, version, iavl_merkle_paths}) => {
      let%Opt encodedRequestPacket = encode(RequestPacket(requestPacket));
      let%Opt encodeRequestPacket = encode(ResponsePacket(responsePacket));
      let encodeVersion = obi_encode_int(version, "u64");
      let%Opt encodeIAVLMerklePaths = encode(IAVLMerklePaths(iavl_merkle_paths));
      Some(
        JsBuffer.concat([|
          encodedRequestPacket,
          encodeRequestPacket,
          encodeVersion,
          encodeIAVLMerklePaths,
        |]),
      );
    }
  | BlockRelayProof({multiStoreProof, blockHeaderMerkleParts, signatures}) => {
      let%Opt encodeMultiStoreProof = encode(MultiStoreProof(multiStoreProof));
      let%Opt encodeBlockHeaderMerkleParts =
        encode(BlockHeaderMerkleParts(blockHeaderMerkleParts));
      let%Opt encodeSignatures = encode(Signatures(signatures));
      Obi.encode(
        {j|{multiStoreProof: bytes, blockHeaderMerkleParts: bytes, signatures: bytes}/{_:u64}|j},
        "input",
        [|
          {
            fieldName: "multiStoreProof",
            fieldValue: encodeMultiStoreProof |> JsBuffer.toHex(~with0x=true),
          },
          {
            fieldName: "blockHeaderMerkleParts",
            fieldValue: encodeBlockHeaderMerkleParts |> JsBuffer.toHex(~with0x=true),
          },
          {
            fieldName: "signatures",
            fieldValue: encodeSignatures |> JsBuffer.toHex(~with0x=true),
          },
        |],
      );
    }
  | Proof({
      blockHeight,
      oracleDataProof: {requestPacket, responsePacket, version, iavl_merkle_paths},
      blockRelayProof: {multiStoreProof, blockHeaderMerkleParts, signatures},
    }) => {
      let%Opt encodeMultiStore = encode(MultiStoreProof(multiStoreProof));
      let%Opt encodeBlockHeaderMerkleParts =
        encode(BlockHeaderMerkleParts(blockHeaderMerkleParts));
      let%Opt encodeSignatures = encode(Signatures(signatures));
      let%Opt encodeReq = encode(RequestPacket(requestPacket));
      let%Opt encodeRes = encode(ResponsePacket(responsePacket));
      let%Opt encodeIAVLMerklePaths = encode(IAVLMerklePaths(iavl_merkle_paths));
      Obi.encode(
        {j|{
          blockHeight: u64,
          multiStore: bytes,
          blockMerkleParts: bytes,
          signatures: bytes,
          packet: bytes,
          version: u64,
          iavlPaths: bytes
        }/{_:u64}|j},
        "input",
        [|
          {fieldName: "blockHeight", fieldValue: blockHeight |> string_of_int},
          {
            fieldName: "multiStore",
            fieldValue: encodeMultiStore |> JsBuffer.toHex(~with0x=true),
          },
          {
            fieldName: "blockMerkleParts",
            fieldValue: encodeBlockHeaderMerkleParts |> JsBuffer.toHex(~with0x=true),
          },
          {
            fieldName: "signatures",
            fieldValue: encodeSignatures |> JsBuffer.toHex(~with0x=true),
          },
          {
            fieldName: "packet",
            fieldValue:
              JsBuffer.concat([|encodeReq, encodeRes|]) |> JsBuffer.toHex(~with0x=true),
          },
          {fieldName: "version", fieldValue: version |> string_of_int},
          {
            fieldName: "iavlPaths",
            fieldValue: encodeIAVLMerklePaths |> JsBuffer.toHex(~with0x=true),
          },
        |],
      );
    };

let int8ToHex: int => string = [%bs.raw
  {|
  function int32ToHex(x) {
    return x.toString(16).padStart(2,0)
  }
|}
];

let int32ToHex: int => string = [%bs.raw
  {|
  function int32ToHex(x) {
    return x.toString(16).padStart(8,0)
  }
|}
];

let int64ToHex: int => string = [%bs.raw
  {|
  function int32ToHex(x) {
    return x.toString(16).padStart(16,0)
  }
|}
];

let encodeString = (s: string) => {
  let buf = JsBuffer.fromUTF8(s);
  JsBuffer.concat([|buf->JsBuffer.byteLength->int32ToHex->JsBuffer.fromHex, buf|]);
};

let encodeBuffer = (buf: JsBuffer.t) =>
  JsBuffer.concat([|buf->JsBuffer.byteLength->int32ToHex->JsBuffer.fromHex, buf|]);

let encodeRequest = (req: request_packet_t) => {
  JsBuffer.concat([|
    req.clientID->encodeString,
    req.oracleScriptID->int64ToHex->JsBuffer.fromHex,
    req.calldata->encodeBuffer,
    req.askCount->int64ToHex->JsBuffer.fromHex,
    req.minCount->int64ToHex->JsBuffer.fromHex,
  |]);
};

let encodeResponse = (res: response_packet_t) => {
  JsBuffer.concat([|
    res.clientID->encodeString,
    res.requestID->int64ToHex->JsBuffer.fromHex,
    res.ansCount->int64ToHex->JsBuffer.fromHex,
    res.requestTime->int64ToHex->JsBuffer.fromHex,
    res.resolveTime->int64ToHex->JsBuffer.fromHex,
    res.resolveStatus->int8ToHex->JsBuffer.fromHex,
    res.result->encodeBuffer,
  |]);
};

let resolveStatusToInt = (rs: RequestSub.resolve_status_t) =>
  switch (rs) {
  | Pending => 0
  | Success => 1
  | Failure => 2
  | Expired => 3
  | Unknown => 4
  };

let toPackets = (request: request_t) => {
  switch (request) {
  | Request({
      clientID,
      oracleScript: {oracleScriptID: ID(oracleScriptID)},
      calldata,
      requestedValidators,
      minCount,
      id: ID(requestID),
      reports,
      requestTime,
      resolveTime,
      resolveStatus,
      result,
    }) => (
      {
        clientID,
        oracleScriptID,
        calldata,
        askCount: requestedValidators |> Belt_Array.length,
        minCount,
      },
      {
        clientID,
        requestID,
        ansCount: reports |> Belt_Array.length,
        requestTime: requestTime |> Belt_Option.getExn |> MomentRe.Moment.toUnix,
        resolveTime: resolveTime |> Belt_Option.getExn |> MomentRe.Moment.toUnix,
        resolveStatus: resolveStatus |> resolveStatusToInt,
        result: result |> Belt_Option.getExn,
      },
    )
  | RequestMini({
      clientID,
      oracleScriptID: ID(oracleScriptID),
      calldata,
      askCount,
      minCount,
      id: ID(requestID),
      reportsCount: ansCount,
      requestTime,
      resolveTime,
      resolveStatus,
      result,
    }) => (
      {clientID, oracleScriptID, calldata, askCount, minCount},
      {
        clientID,
        requestID,
        ansCount,
        requestTime: requestTime |> Belt_Option.getExn |> MomentRe.Moment.toUnix,
        resolveTime: resolveTime |> Belt_Option.getExn |> MomentRe.Moment.toUnix,
        resolveStatus: resolveStatus |> resolveStatusToInt,
        result: result |> Belt_Option.getExn,
      },
    )
  };
};

let createProof = (proof: Js.Json.t) => {
  Proof(proof |> decodeProof) |> encode |> Belt_Option.getExn;
};

let createProof_2 = (request: request_t) => {
  let (req, res) = request->toPackets;
  JsBuffer.concat([|req->encodeRequest, res->encodeResponse|]);
};
