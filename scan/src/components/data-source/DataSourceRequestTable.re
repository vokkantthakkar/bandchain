module Styles = {
  open Css;

  let tableWrapper = style([padding2(~v=`px(20), ~h=`px(15))]);

  let txContainer = style([width(`px(230)), cursor(`pointer)]);

  let icon = style([width(`px(80)), height(`px(80))]);
  let iconWrapper =
    style([
      width(`percent(100.)),
      display(`flex),
      flexDirection(`column),
      alignItems(`center),
    ]);
};

type oracle_script_t = {
  id: int,
  description: string,
};

[@react.component]
let make = (~dataSourceID: ID.DataSource.t) =>
  {
    let (page, setPage) = React.useState(_ => 1);
    let pageSize = 5;

    Js.Console.log("zzzzzzzz");

    let requestsSub = RequestSub.Mini.getListByDataSource(dataSourceID, ~pageSize, ~page, ());
    let totalRequestCountSub = RequestSub.countByDataSource(dataSourceID);

    Js.Console.log("ccccccccc");

    Js.Console.log(requestsSub);
    Js.Console.log(totalRequestCountSub);

    switch (totalRequestCountSub) {
    | ApolloHooks.Subscription.Data(data) => Js.Console.log(data)
    | Loading => Js.Console.log("loading")
    | Error(e) => Js.Console.log2("--->", e)
    | NoData => Js.Console.log("no data")
    };

    // let%Sub requests = requestsSub;
    // let%Sub totalRequestCount = totalRequestCountSub;
    let requests: array(BandScan.RequestSub.Mini.t) = [||];
    let totalRequestCount = 100;

    Js.Console.log("bbbbbbbbb");

    let pageCount = Page.getPageCount(totalRequestCount, pageSize);

    <div className=Styles.tableWrapper>
      <Row>
        <HSpacing size={`px(25)} />
        <Text value={totalRequestCount |> string_of_int} weight=Text.Bold />
        <HSpacing size={`px(5)} />
        <Text value="Requests" />
      </Row>
      <VSpacing size=Spacing.lg />
      {totalRequestCount > 0
         ? <>
             <THead>
               <Row>
                 <Col> <HSpacing size=Spacing.lg /> </Col>
                 <Col size=1.>
                   <div className=TElement.Styles.hashContainer>
                     <Text
                       block=true
                       value="REQUEST"
                       size=Text.Sm
                       weight=Text.Bold
                       color=Colors.gray6
                     />
                   </div>
                 </Col>
                 <Col size=2.8>
                   <Text
                     block=true
                     value="ORACLE SCRIPT"
                     size=Text.Sm
                     weight=Text.Bold
                     color=Colors.gray6
                   />
                 </Col>
                 <Col size=2.>
                   <Text block=true value="AGE" size=Text.Sm weight=Text.Bold color=Colors.gray6 />
                 </Col>
                 <Col size=1.5>
                   <Text
                     block=true
                     value="BLOCK"
                     size=Text.Sm
                     weight=Text.Bold
                     color=Colors.gray6
                   />
                 </Col>
                 <Col size=2.7>
                   <Text
                     block=true
                     value="TX HASH"
                     size=Text.Sm
                     weight=Text.Bold
                     color=Colors.gray6
                   />
                 </Col>
                 <Col> <HSpacing size=Spacing.lg /> </Col>
               </Row>
             </THead>
             {requests
              ->Belt_Array.map(
                  ({id, oracleScriptID, oracleScriptName, timestamp, blockHeight, txHash}) => {
                  <TBody key={txHash |> Hash.toHex(~upper=true)}>
                    <Row>
                      <Col> <HSpacing size=Spacing.lg /> </Col>
                      <Col size=1.> <TypeID.Request id /> </Col>
                      <Col size=2.8>
                        <Row>
                          <TypeID.OracleScript id=oracleScriptID />
                          <HSpacing size={`px(5)} />
                          <Text
                            block=true
                            value=oracleScriptName
                            weight=Text.Medium
                            color=Colors.gray7
                          />
                        </Row>
                      </Col>
                      <Col size=2.>
                        <TimeAgos time=timestamp size=Text.Md weight=Text.Medium />
                      </Col>
                      <Col size=1.5> <TypeID.Block id=blockHeight /> </Col>
                      <Col size=2.7>
                        <div
                          className=Styles.txContainer
                          onClick={_ => Route.redirect(Route.TxIndexPage(txHash))}>
                          <Text
                            block=true
                            value={txHash |> Hash.toHex(~upper=true)}
                            weight=Text.Medium
                            code=true
                            color=Colors.gray7
                            ellipsis=true
                            nowrap=true
                          />
                        </div>
                      </Col>
                      <Col> <HSpacing size=Spacing.lg /> </Col>
                    </Row>
                  </TBody>
                })
              ->React.array}
           </>
         : <div className=Styles.iconWrapper>
             <VSpacing size={`px(30)} />
             <img src=Images.noRequestIcon className=Styles.icon />
             <VSpacing size={`px(40)} />
             <Text block=true value="NO REQUEST" weight=Text.Regular color=Colors.blue4 />
             <VSpacing size={`px(15)} />
           </div>}
      <VSpacing size=Spacing.lg />
      <Pagination currentPage=page pageCount onPageChange={newPage => setPage(_ => newPage)} />
      <VSpacing size=Spacing.lg />
    </div>
    |> Sub.resolve;
  }
  |> Sub.default(_, React.null);
