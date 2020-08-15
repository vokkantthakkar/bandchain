module Styles = {
  open Css;

  let container =
    style([
      padding2(~v=`px(40), ~h=`px(45)),
      Media.mobile([padding2(~v=`px(20), ~h=`zero)]),
    ]);

  let upperTextCotainer = style([marginBottom(`px(24))]);

  let paramsContainer = style([display(`flex), flexDirection(`column)]);

  let listContainer = style([marginBottom(`px(25))]);

  let withPadding = (h, v) => style([padding2(~h=`px(h), ~v=`px(v))]);

  let input =
    style([
      width(`percent(100.)),
      background(white),
      padding2(~v=`zero, ~h=`px(16)),
      fontSize(`px(12)),
      fontWeight(`num(500)),
      outline(`px(1), `none, white),
      height(`px(37)),
      borderRadius(`px(4)),
      border(`px(1), `solid, Colors.gray9),
      placeholder([color(Colors.blueGray3)]),
    ]);

  let buttonContainer = style([display(`flex), flexDirection(`row), alignItems(`center)]);

  let button = isLoading =>
    style([
      width(`px(isLoading ? 150 : 110)),
      backgroundColor(isLoading ? Colors.blueGray3 : Colors.green2),
      borderRadius(`px(6)),
      fontSize(`px(12)),
      fontWeight(`num(600)),
      color(isLoading ? Colors.blueGray7 : Colors.green7),
      cursor(isLoading ? `auto : `pointer),
      padding2(~v=Css.px(10), ~h=Css.px(10)),
      whiteSpace(`nowrap),
      outline(`zero, `none, white),
      boxShadow(
        isLoading
          ? `none : Shadow.box(~x=`zero, ~y=`px(2), ~blur=`px(4), Css.rgba(0, 0, 0, 0.1)),
      ),
      border(`zero, `solid, Colors.white),
    ]);

  let hFlex = h =>
    style([display(`flex), flexDirection(`row), alignItems(`center), height(h)]);

  let vFlex = (w, h) => style([display(`flex), flexDirection(`column), width(w), height(h)]);

  let withWH = (w, h) =>
    style([
      width(w),
      height(h),
      display(`flex),
      justifyContent(`center),
      alignItems(`center),
    ]);

  let resultWrapper = (w, h, paddingV, overflowChioce) =>
    style([
      width(w),
      height(h),
      display(`flex),
      flexDirection(`column),
      padding2(~v=paddingV, ~h=`zero),
      justifyContent(`center),
      backgroundColor(Colors.white),
      borderRadius(`px(4)),
      overflow(overflowChioce),
    ]);

  let logo = style([width(`px(15))]);
  let separatorLine =
    style([
      borderStyle(`none),
      backgroundColor(Colors.gray9),
      height(`px(1)),
      margin3(~top=`px(10), ~h=`zero, ~bottom=`px(20)),
    ]);
  let titleSpacing = style([marginBottom(`px(8))]);
  let infoIcon = style([width(`px(12)), height(`px(12)), display(`block)]);
};

let parameterInput = (Obi.{fieldName, fieldType}, index, setCalldataArr) => {
  let fieldName = Js.String.replaceByRe([%re "/[_]/g"], " ", fieldName);
  <div className=Styles.listContainer key=fieldName>
    <div className={CssHelper.flexBox()}>
      <Text
        value=fieldName
        size=Text.Md
        color=Colors.gray7
        weight=Text.Semibold
        transform=Text.Capitalize
      />
      <HSpacing size=Spacing.xs />
      <Text value={j|($fieldType)|j} size=Text.Md color=Colors.gray7 weight=Text.Semibold />
    </div>
    <VSpacing size=Spacing.sm />
    <input
      className=Styles.input
      type_="text"
      onChange={event => {
        let newVal = ReactEvent.Form.target(event)##value;
        setCalldataArr(prev => {
          prev->Belt_Array.mapWithIndex((i, value) => {index == i ? newVal : value})
        });
      }}
    />
  </div>;
};

let countInputs = (askCount, setAskCount, setMinCount) => {
  <Row.Grid>
    <Col.Grid col=Col.Two colSm=Col.Six>
      <div className={Css.merge([CssHelper.flexBox(), Styles.titleSpacing])}>
        <Text
          value="Ask Count"
          size=Text.Md
          color=Colors.gray7
          weight=Text.Semibold
          transform=Text.Capitalize
        />
        <HSpacing size=Spacing.xs />
        //TODO: remove mock message later
        <CTooltip
          tooltipPlacementSm=CTooltip.BottomLeft
          tooltipText="Lorem ipsum, or lipsum as it is sometimes known.">
          <img className=Styles.infoIcon src=Images.infoIcon />
        </CTooltip>
      </div>
      <select
        className=Styles.input
        onChange={event => {
          let newVal = ReactEvent.Form.target(event)##value;
          setAskCount(_ => newVal);
        }}>
        {[|1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16|]
         |> Belt.Array.map(_, index =>
              <option key={(index |> string_of_int) ++ "askCount"} value={index |> string_of_int}>
                {index |> string_of_int |> React.string}
              </option>
            )
         |> React.array}
      </select>
    </Col.Grid>
    <Col.Grid col=Col.Two colSm=Col.Six>
      <div className={Css.merge([CssHelper.flexBox(), Styles.titleSpacing])}>
        <Text
          value="Min Count"
          size=Text.Md
          color=Colors.gray7
          weight=Text.Semibold
          transform=Text.Capitalize
        />
        <HSpacing size=Spacing.xs />
        //TODO: remove mock message later
        <CTooltip
          tooltipPlacementSm=CTooltip.BottomLeft
          tooltipText="Lorem ipsum, or lipsum as it is sometimes known.">
          <img className=Styles.infoIcon src=Images.infoIcon />
        </CTooltip>
      </div>
      <select
        className=Styles.input
        onChange={event => {
          let newVal = ReactEvent.Form.target(event)##value;
          setMinCount(_ => newVal);
        }}>
        {Belt.Array.makeBy(askCount |> int_of_string, i => i + 1)
         |> Belt.Array.map(_, index =>
              <option key={(index |> string_of_int) ++ "minCount"} value={index |> string_of_int}>
                {index |> string_of_int |> React.string}
              </option>
            )
         |> React.array}
      </select>
    </Col.Grid>
  </Row.Grid>;
};

let clientIDInput = setClientID => {
  <div className=Styles.listContainer>
    <Text
      value="Client ID"
      size=Text.Md
      color=Colors.gray7
      weight=Text.Semibold
      transform=Text.Capitalize
    />
    <VSpacing size=Spacing.sm />
    <input
      className=Styles.input
      type_="text"
      onChange={event => {
        let newVal = ReactEvent.Form.target(event)##value;
        setClientID(_ => newVal);
      }}
    />
  </div>;
};

type result_t =
  | Nothing
  | Loading
  | Error(string)
  | Success(TxCreator.tx_response_t);

let loadingRender = (wDiv, wImg, h) => {
  <div className={Styles.withWH(wDiv, h)}>
    <img src=Images.loadingCircles className={Styles.withWH(wImg, h)} />
  </div>;
};

let resultRender = (result, schema) => {
  switch (result) {
  | Nothing => React.null
  | Loading =>
    <>
      <VSpacing size=Spacing.xl />
      {loadingRender(`percent(100.), `px(104), `px(30))}
      <VSpacing size=Spacing.lg />
    </>
  | Error(err) =>
    <>
      <VSpacing size=Spacing.lg />
      <div className={Styles.resultWrapper(`percent(100.), `px(90), `zero, `scroll)}>
        <Text value=err />
      </div>
    </>
  | Success(txResponse) => <OracleScriptExecuteResponse txResponse schema />
  };
};

module ExecutionPart = {
  [@react.component]
  let make = (~id: ID.OracleScript.t, ~schema: string, ~paramsInput: array(Obi.field_key_type_t)) => {
    let (_, dispatch) = React.useContext(AccountContext.context);

    let numParams = paramsInput->Belt_Array.size;

    // let%Sub validatorCount = ValidatorSub.count();

    let (callDataArr, setCallDataArr) = React.useState(_ => Belt_Array.make(numParams, ""));
    let (clientID, setClientID) = React.useState(_ => "from_band");
    let (askCount, setAskCount) = React.useState(_ => "1");
    let (minCount, setMinCount) = React.useState(_ => "1");
    let (result, setResult) = React.useState(_ => Nothing);

    Js.Console.log2("Ask Count: ", askCount);
    Js.Console.log2("Min Count: ", minCount);

    // TODO: Change when input can be empty
    let isUnused = {
      let field = paramsInput->Belt_Array.getExn(0);
      field.fieldName |> Js.String.startsWith("_");
    };
    React.useEffect0(() => {
      if (isUnused) {
        setCallDataArr(_ => [|"0"|]);
      };
      None;
    });

    let requestCallback =
      React.useCallback0(requestPromise => {
        ignore(
          requestPromise
          |> Js.Promise.then_(res =>
               switch (res) {
               | TxCreator.Tx(txResponse) =>
                 setResult(_ => Success(txResponse));
                 Js.Promise.resolve();
               | _ =>
                 setResult(_ =>
                   Error("Fail to sign message, please connect with mnemonic or ledger first")
                 );
                 Js.Promise.resolve();
               }
             )
          |> Js.Promise.catch(err => {
               switch (Js.Json.stringifyAny(err)) {
               | Some(errorValue) => setResult(_ => Error(errorValue))
               | None => setResult(_ => Error("Can not stringify error"))
               };
               Js.Promise.resolve();
             }),
        );
        ();
      });
    <Row.Grid>
      <Col.Grid>
        <div className=Styles.container>
          {isUnused
             ? React.null
             : <div>
                 <div className={Css.merge([CssHelper.flexBox(), Styles.upperTextCotainer])}>
                   <Text
                     value="This oracle script requires the following"
                     color=Colors.gray7
                     size=Text.Lg
                   />
                   <HSpacing size=Spacing.sm />
                   {numParams == 0
                      ? React.null
                      : <Text
                          value={numParams > 1 ? "parameters" : "parameter"}
                          color=Colors.gray7
                          weight=Text.Bold
                          size=Text.Lg
                        />}
                 </div>
                 <VSpacing size=Spacing.lg />
                 <div className=Styles.paramsContainer>
                   {paramsInput
                    ->Belt_Array.mapWithIndex((i, param) =>
                        parameterInput(param, i, setCallDataArr)
                      )
                    ->React.array}
                 </div>
               </div>}
          <div> {clientIDInput(setClientID)} </div>
          <hr className=Styles.separatorLine />
          {countInputs(askCount, setAskCount, setMinCount)}
          <VSpacing size=Spacing.md />
          <div className=Styles.buttonContainer>
            <button
              className={Styles.button(result == Loading)}
              onClick={_ =>
                if (result != Loading) {
                  switch (
                    Obi.encode(
                      schema,
                      "input",
                      paramsInput
                      ->Belt_Array.map(({fieldName}) => fieldName)
                      ->Belt_Array.zip(callDataArr)
                      ->Belt_Array.map(((fieldName, fieldValue)) =>
                          Obi.{fieldName, fieldValue}
                        ),
                    )
                  ) {
                  | Some(encoded) =>
                    setResult(_ => Loading);
                    dispatch(
                      AccountContext.SendRequest(
                        id,
                        encoded,
                        requestCallback,
                        askCount,
                        minCount,
                        clientID,
                      ),
                    );
                    ();
                  | None =>
                    setResult(_ => Error("Encoding fail, please check each parameter's type"))
                  };
                  ();
                }
              }>
              {(result == Loading ? "Sending Request ... " : "Request") |> React.string}
            </button>
          </div>
          {resultRender(result, schema)}
        </div>
      </Col.Grid>
    </Row.Grid>;
  };
};

[@react.component]
let make = (~id: ID.OracleScript.t, ~schema: string) =>
  {
    let%Opt paramsInput = schema->Obi.extractFields("input");
    Some(<ExecutionPart id schema paramsInput />);
  }
  |> Belt.Option.getWithDefault(
       _,
       <div className={Styles.withPadding(20, 20)}>
         <Text value="Schema not found" color=Colors.gray7 />
       </div>,
     );
