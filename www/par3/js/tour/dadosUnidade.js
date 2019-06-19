$(document).ready(function() {
	var tour = new Tour({
		name: "tour-dados-unidade",
		backdrop: true,
		backdropPadding: 0,
		onHide: function() {
			$('.tour-tour-dados-unidade').hide();
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
			orphan: true,
			title: "Abertura da Visita Guiada",
			content: "<p>Preparamos um rápida visita guiada ao novo sistema!</p> " +
					 "<p>Para começar agora, clique <a href='#' data-role='next'>AQUI</a>. </p>" +
					 "<p>Caso prefira conhecê-lo em outro momento, basta clicar em no botão " +
			         "<a class='btn_par-floating btn_par-large red' style='margin: 0px 5px; height: 25px; width: 25px; line-height: 29px;'>" +
			         "	<i class='large material-icons' style='font-size: 14px; line-height: 26px;'>view_headline</i>" +
				     "</a>" +
			         ". Após isto, clique no botão" +
					 "<a class='btn_par-floating tourn-btn indigo' style='transform: scaleX(1) scaleY(1) translateY(0px) translateX(0px); opacity: 1;margin: 0px 5px; height: 25px; width: 25px; line-height: 29px;'>" +
					 "  <i class='material-icons' style='font-size: 14px; line-height: 26px;'>language</i>" +
					 "</a>" +
					 " que estará sempre disponível no canto inferior direito da sua tela. Você poderá acessá-lo quantas vezes desejar e a qualquer momento durante os seus acessos!</p> ",
			template: "<div class='popover tour' style='max-width: 780px; font-size: 14px;'>" +
					  "	<div class='arrow'></div>" +
					  "		<h3 class='popover-title' style='font-size: 16px; font-weight: bold'></h3>" +
					  "		<div class='popover-content' style='padding: 15px;'></div>" +
					  "		<div class='popover-navigation text-right'>" +
					  "			<button class='btn btn-default' data-role='next'>Sim</button>" +
				      "			<span data-role='separator'>&nbsp;</span>" +
					  "			<button class='btn btn-default' data-role='end'>Não</button>" +
					  "		</div>" +
					  "</div>"
		},
		{
			placement: "bottom",
			element: ".linhaWizard",
			title: "Apresentação Geral",
			content: "<p>Nesta primeira etapa, que envolve o diagnóstico da rede de ensino, o sistema apresenta os 6 ícones que estão em destaque.</p>" +
		 			 "<p>A seguir, faremos uma visita guiada em cada um desses ícones.</p>"
		},
		{
			placement: "bottom",
			element: "#btnDadosUnidade",
			title: "Dados da Unidade",
			content: "<p>No primeiro ícone, Dados da Unidade, serão preenchidas as informações cadastrais de dirigentes, técnicos e conselhos, de maneira a indicar os representantes de cada posição e garantir acesso a funcionalidades específicas do sistema para cada um desses perfis.</p>" +
					 "<p>O cadastramento completo dos dados e a sua constante atualização são imprescindíveis para garantir que todas as comunicações feitas pelo sistema cheguem aos seus destinatários.</p>"
		},
		{
			placement: "bottom",
			element: "#btnPNE",
			title: "PNE",
			content: "<p>No segundo ícone, Plano Nacional de Educação (PNE), serão registradas as metas dos respectivos Planos de cada ente federado, com relação às vinte metas do PNE.  Cada ente federativo apresentará sua colaboração para que as metas nacionais sejam atingidas. A partir dessas informações, o Ministério da Educação direcionará suas ações, programas e projetos, unificando esforços para que o país avance nas prioridades definidas pela Lei 13.005/2014 (Lei do PNE).</p>" +
					 "<p>É muito importante, durante o preenchimento, estar atento para que as metas do plano estadual, distrital ou municipal estejam compatibilizadas com as metas do PNE, caracterizando a consonância exigida pela lei nacional.</p>"
		},
		{
			placement: "bottom",
			element: "#btnQuestoesEstrategicas",
			title: "Questões Estratégicas",
			content: "<p>No ícone das Questões Estratégicas, os itens integram uma parte importante do diagnóstico da situação educacional local, onde municípios, estados e o Distrito Federal apresentam dados considerados fundamentais para uma gestão estratégica bem sucedida em suas respectivas redes.</p>" +
					 "<p>Há itens para os quais é possível assinalar somente uma alternativa de resposta, há outros que admitem duas ou mais opções. Dependendo do que for assinalado, será necessário anexar um arquivo pertinente ao que foi respondido.</p>" +
					 "<p>Na aba, Plano Nacional de Educação: Questões Complementares, as informações registradas pelos entes federados auxiliarão no monitoramento do alcance das metas do PNE, a partir de suas respectivas estratégias.</p>"
		},
		{
			placement: "bottom",
			element: "#btnAcompanhamento",
			title: "Execução e Acompanhamento",
			content: "<p>O ícone Execução e Acompanhamento apresenta uma tela espelho daquela mostrada no ciclo do PAR 2011-2014, ou seja, é apenas uma cópia daquela tela e objetiva informar municípios e estados/Distrito Federal sobre sua situação em relação aos Termos de Compromisso; e sobre eles, informamos que qualquer interação com o sistema continua sendo realizada pelo PAR 2011-2014.</p>" +
					 "<p>Há ainda o Quadro do SIOPE, que apresenta quais são as receitas destinadas à educação no município ou no estado/Distrito Federal, aonde elas são aplicadas (despesas) e se estão sendo cumpridos os percentuais exigidos em lei.</p>",
		},
		{
			placement: "bottom",
			element: "#btnPendencias",
			title: "Pendências",
			content: "<p>O ícone Pendências informa se há e quais são as pendências dos municípios, estados/Distrito Federal, relacionadas ao MEC/FNDE, que necessitam a atenção dos gestores antes de avançarem para o Diagnóstico</p>"
		},
		{
			placement: "bottom",
			element: "#btnIndicadoresQualitativos",
			title: "Diagnóstico",
			content: "<p>O diagnóstico tem por objetivo promover uma ampla reflexão acerca da situação educacional da rede municipal ou estadual/distrital. Assim, a coleta de informações e o seu detalhamento deverão ser realizados em conjunto pela equipe técnica local.</p>" +
				     "<p>Ao clicar neste ícone o usuário avançará para o início do seu Diagnóstico. Caso tenha esquecido de preencher alguns dos campos obrigatórios dos ícones anteriores, no entanto, o sistema emitirá um alerta e mostrará quais os campos estão faltando ser preenchidos.</p>",
		},
	]});

	$('.tourn-btn').on('click', function() {
		tour.restart();
	})
})