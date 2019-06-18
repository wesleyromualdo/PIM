$(document).ready(function() {
    var tour = new Tour({
        name: "tour-pne",
        backdrop: true,
        backdropPadding: 0,
        template: "<div class='popover tour'>" +
                  "	<div class='arrow'></div>" +
                  "		<h3 class='popover-title'></h3>" +
                  "		<div class='popover-content'></div>" +
                  "		<div class='popover-navigation'>" +
                  "			<button class='btn btn-sm btn-default' data-role='prev'>« Anterior</button>" +
                  "			<button class='btn btn-sm btn-default' data-role='next'>Proximo »</button>" +
                  "			<span data-role='separator'>&nbsp;</span>" +
                  "			<button class='btn btn-sm btn-default' data-role='end'>Finalizar</button>" +
                  "		</div>" +
                  "</div>",
        steps: [
            {
                element: "#pneArquivo",
                title: "Arquivo do PPE",
                content: "Para fazer download da lei municipal/estadual PPE basta clicar aqui."
            },
            {
                element: ".btn-primary-active",
                title: "Metas",
                content: "Para trocar de meta basta clicar no botão da meta que deseja acessar."
            },
            {
                element: "#highcharts-0",
                title: "Meta Brasil",
                content: "Este é o grafico que mostra os valores da meta Brasil."
            },
            {
                element: ".slider-range[0]",
                title: "Valor da Meta",
                content: "Para configurar o valor da meta basta arrastar ate o valor indicado ou preencher o campo ao lado."
            },
            {
                element: ".div_combo_grfAno[0]",
                title: "Ano Previsto",
                content: "Para trocar o ano de configuração da meta basta selecionar o ano que deseja nesta lista."
            },
            {
                element: "#btnNaoInfo",
                title: "Não Informado",
                content: "Caso esta meta não possua valor pode configura-la como não informada clicando neste botão.",
                onNext: function() {
                    document.location.href = "par3.php?modulo=principal/planoTrabalho/questoesEstrategicas&acao=A&inuid=3459";
                }
            },
            {
                orphan: true,
                title: "Questoes Estrategicas",
                content: "Aguarde redirecionando...",
                container: "body",
            }
        ]
    });

    $('.tourn-btn').on('click', function() {
        tour.restart();
    })
})