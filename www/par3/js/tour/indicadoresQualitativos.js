$(document).ready(function() {
    var tour = new Tour({
        name: "tour-diagnostico",
        backdrop: true,
        storage: window.localStorage,
        backdropPadding: 0,
        onHide: function() {
            $('.tour-tour-diagnostico').hide();
        },
        onShow: function() {
            $('#loading').css('display', 'none');
        },
        template: "<div class='popover tour' style='max-width: 570px;'>" +
        "	<div class='arrow'></div>" +
        "		<h3 class='popover-title'></h3>" +
        "		<div class='popover-content'></div>" +
        "		<div class='popover-navigation'>" +
        "			<button class='btn btn-default' data-role='prev'>« Anterior</button>" +
        "			<button class='btn btn-default' data-role='next'>Proximo »</button>" +
        "			<span data-role='separator'>&nbsp;</span>" +
        "			<button class='btn btn-default' data-role='end'>Finalizar</button>" +
        "		</div>" +
        "</div>",
        steps: [
        {
            placement: "bottom",
            element: ".nav-tabs",
            title: "Dimensões",
            content: "<p>O diagnóstico está estruturado em quatro grandes dimensões</p>" +
                     "<ul>" +
                     "  <li>Dimensão 1: Gestão Educacional</li>" +
                     "  <li>Dimensão 2: Formação de Profissionais da Educação</li>" +
                     "  <li>Dimensão 3: Práticas Pedagógicas e Avaliação</li>" +
                     "  <li>Dimensão 4: Infraestrutura Física e Recursos Pedagógicos</li>" +
                     "</ul>"
        },
        {
            placement: "bottom",
            element: ".menuAreaTabelaSiope",
            title: "SIOPE",
            content: "<p>O Quadro do SIOPE apresenta quais são as receitas destinadas à educação no município ou no estado/Distrito Federal, aonde elas são aplicadas (despesas) e se estão sendo cumpridos os percentuais exigidos em lei.</p>"
        },
        {
            placement: "bottom",
            element: ".tabIndicadoresDemonstrativo",
            title: "Adequação da Formação Docente",
            content: "<p>O Quadro da Formação Docente apresenta dados do Censo Escolar relacionados à adequação da formação dos docentes da rede em relação à disciplina que lecionam.</p>"
        },
        {
            placement: "bottom",
            element: ".tabIndicadoresGrafico",
            title: "PDDE",
            content: "<p>O Quadro das Condições de Uso das Dependências Escolares reúne dados do Censo Escolar com dados que as escolas da rede informaram no PDDE Interativo. Assim, apresenta ao gestor da rede um retrato do que foi informado em relação às condições de uso das dependências escolares.</p>",
        },
    ]});

    $('.tourn-btn').on('click', function() {
        tour.restart();
    })
})