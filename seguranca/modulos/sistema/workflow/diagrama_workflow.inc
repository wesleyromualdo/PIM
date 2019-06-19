 <?php 
 
    global $db;
    
    //filtrando parâmetro
    $tpdid = filter_input(INPUT_GET, 'tpdid', FILTER_SANITIZE_NUMBER_INT);
    $filtro = filter_input(INPUT_POST,'filtros', FILTER_SANITIZE_NUMBER_INT, FILTER_REQUIRE_ARRAY);
    
    //verificando se um id foi devidamente informado 
    if (empty($tpdid)) {
        die("O ID do fluxo informado é inválido");    
    }
    
    //consulta para obter os dados do fluxo usados no cabeçalho da página
    $sqlFluxo =
    "SELECT
        fluxo.tpdid,
        fluxo.tpddsc,
        sis.sisdsc
	
    FROM	
        workflow.tipodocumento AS fluxo
        
    LEFT JOIN
        seguranca.sistema AS sis ON (fluxo.sisid = sis.sisid)
        
    WHERE
        tpdid = {$tpdid}";
    
    
   
    
    //consulta para buscar as ações
    $sqlAcoes =
    "SELECT DISTINCT
           acao.esdidorigem,
	       acao.esdiddestino,
           acao.aeddscrealizar,
           acao.aedvisivel, 
           acao.aedcondicao,
           acao.esdcomentarioobrigatorio,
           acao.aeddescregracondicao,
           array_to_string(array_agg(perfil.pfldsc), '#') as perfis

    FROM
       workflow.acaoestadodoc AS acao
       
    JOIN
       workflow.estadodocumento AS estado ON (acao.esdidorigem = estado.esdid OR acao.esdiddestino = estado.esdid)
       
    LEFT JOIN
			workflow.estadodocumentoperfil AS permissao ON (acao.aedid = permissao.aedid)
			
	LEFT JOIN
			seguranca.perfil AS perfil ON (permissao.pflcod = perfil.pflcod)

    WHERE
       estado.tpdid = {$tpdid}";
    
    //concatenando filtro pelo array de estados selecionados se não for vazio
    if (!empty($filtro)) {
        $sqlAcoes .= " AND
        estado.esdid IN (".implode(",",$filtro).") ";
    }
    
    $sqlAcoes .=
    " GROUP BY
       acao.esdidorigem,
       acao.esdiddestino,
       acao.aeddscrealizar,
       acao.aedvisivel, 
       acao.aedcondicao,
       acao.esdcomentarioobrigatorio,
       acao.aeddescregracondicao";
    
    $acoes = $db->carregar($sqlAcoes);
        
    $arcos = array();
       
    //preparando o array de arcos que será convertido para JSON
    foreach($acoes as $i => $acao) {
        
        $arcos[$i]['to'] = $acao['esdiddestino']; // nó de destino
        $arcos[$i]['from'] = $acao['esdidorigem']; // nó de origem
        //$arcos[$i]['label'] = $acao['aeddscrealizar']; // texto da ação
        $arcos[$i]['arrows'] = 'to'; // exibe setas no destino da linha
        
        //se a ação não for visível exibe em cinza
        if (empty($acao['aedvisivel']) || $acao['aedvisivel'] == 'f') {
            $arcos[$i]['color'] = 'grey';
        }
        
        //se a ção tiver condicação exibe a linha pontilhada
        if (!empty(trim($acao['aedcondicao']))) {
            $arcos[$i]['dashes'] = 'true';
        }
        
        $perfis = explode('#',$acao['perfis']);
        
        $nome = "<p><strong>{$acao['aeddscrealizar']}</strong></p>";
        
        if (!empty($perfis)) {
            $perfisStr = "<p><strong>Perfis:</strong><br> ".implode(";<br>",$perfis)."</p>";
        }
        
        if ($acao['aedvisivel'] == 't') {
            $visivelStr = "Sim";
        } else {
            $visivelStr = "Não";
        }
        
        $visivel = "<p><strong>Visível:</strong> {$visivelStr}</p>";
        
        //reiniciando as variáveis a cada loop para não repetir o texto da iteração anterior
        $condicao = null;
        $condicaoDesc = null;
        $preacao = null;
        $posacao = null;
        $comentario = null;
        
        //adicionando nome da condição, se houver
        if (!empty(trim($acao['aedcondicao']))) {
            $condicao = "<p><strong>Condição:</strong> {$acao['aedcondicao']}</p>";
        } 
        
        //adicionando descrição da condição, se houver
        if (!empty(trim($acao['aeddescregracondicao']))) {
            $condicaoDesc = "<p><strong>Descrição Condição:</strong> {$acao['aeddescregracondicao']}</p>";
        } 
        
        
        //adicionando nome da Prá-ação, se houver
        if (!empty(trim($acao['aedpreacao']))) {
            $preacao = "<p><strong>Pré-Ação:</strong> {$acao['aedpreacao']}</p>";
        } 
        
        //adicionando nome da Pós-ação, se houver
        if (!empty(trim($acao['aedposacao']))) {
            $posacao = "<p><strong>Pós-Ação:</strong> {$acao['aedposacao']}</p>";
        } 
        
        //adicionando nome da Pós-ação, se houver
        if (!empty(trim($acao['esdcomentarioobrigatorio']))) {
            $comentario = "<p><strong>Comentário:</strong> Sim</p>";
        } 
        
        //interpolação de string com o tooltipo com dados adicionais da ação (quebras de linha causam erro no parse do json)
        $arcos[$i]['title'] = "{$nome} {$visivel} {$condicao} {$condicaoDesc} {$comentario} {$perfisStr}"; 
    }
    
    /* usando a função array_column para recuperar os estados que interagem via alguma ação com os estados selecionados,
    sejam de origem ou de destino */
    $estadosDestino = array_column($acoes, 'esdiddestino');
    $estadosOrigem = array_column($acoes, 'esdidorigem');
    
    //mesclando os dois arrays
    $estadosEncontrados = array_merge($estadosDestino, $estadosOrigem);
    
    /*consulta de estados, já filtra pelos estados encontrados anteriormente para não trazer nenhum estado além dos presentes no filtro
    e aqueles que tem alguma ação relacionada à eles.*/
    $sqlEstados = 
    "SELECT 
        esdid AS id,
	    esddsc AS label          
    FROM
       workflow.estadodocumento
    
    WHERE
       tpdid = {$tpdid}
    
    AND
        esdid IN (".implode(",",$estadosEncontrados).")";
    
  
    
    $fluxo = $db->pegaLinha($sqlFluxo);
    $estados = $db->carregar($sqlEstados);
    
    
    //convertendo para json
    $estados = simec_json_encode($estados);
    $acoes = simec_json_encode($arcos);
 ?>



 <link rel="stylesheet" href="//maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
 <script type="text/javascript" src="//cdnjs.cloudflare.com/ajax/libs/vis/4.20.1/vis.min.js"></script>
 <script type="text/javascript" src="/includes/JQuery/jquery-1.9.1/jquery-1.9.1.js"></script>
 <script type="text/javascript" src="/library/chosen-1.0.0/chosen.jquery.js"></script>
 <link href="/library/chosen-1.0.0/chosen.css" rel="stylesheet" type="text/css" />
 <link href="//cdnjs.cloudflare.com/ajax/libs/vis/4.20.1/vis.min.css" rel="stylesheet" type="text/css" />
 <link rel="stylesheet" href="//cdnjs.cloudflare.com/ajax/libs/jquery.sumoselect/3.0.2/sumoselect.min.css">
 <script src="//cdnjs.cloudflare.com/ajax/libs/jquery.sumoselect/3.0.2/jquery.sumoselect.js" ></script>
 <script src="//maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js" ></script>

    <style type="text/css">
        #mynetwork {
            width: 1200px;
            height: 650px;
            border: 1px solid lightgray;
        }
        .oculto {
            display: none;
        }
    </style>

<body>

<nav class="breadcrumb">
  <a class="breadcrumb-item" href="#"><?=$fluxo['sisdsc'];?></a>
  <span class="breadcrumb-item active"><?=$fluxo['tpddsc'];?> ID#<?=$fluxo['tpdid'];?> &nbsp; 
  <button id="btn_mais" class="btn btn-primary">Filtros</button></span>
  <button id="btn_config" class="btn btn-primary">Configurações</button><br><br></span>
</nav>

 

 <div id="filtros" <?= empty($filtro) ? 'class="oculto"' : ''; ?>>
    <form method="post">
        <!--  Filtrar pelos estados:<br/>-->
        <?php 
            $sqlSelect = 
            "SELECT 
                    esdid AS codigo,
                    esddsc AS descricao
             FROM
                    workflow.estadodocumento
             WHERE
                    tpdid = {$tpdid} ";
            
            $db->monta_combo("filtros[]", $sqlSelect, "S", null, '', '', '', '', 'S', 'estado', '', $filtro, null, ' multiple ', 'multi');
        ?>
        <br><br>          
        <button type="submit" class="btn btn-primary" style="z-index: 999;">Filtrar</button>
    </form>
 </div>


<div id="config" style="margin: 0 auto;" class="oculto"></div>

<div id="mynetwork" style="margin: 0 auto;"></div>

<br><br>

<div class="container">
	<div class="alert alert-info alert-dismissable fade in">
    <a href="#" class="close" data-dismiss="alert" aria-label="close">&times;</a>
    <strong>OBS:</strong> Para interromper a movimentação do gráfico, desmarque a opção "enabled" das configurações de física.
  </div>
</div>

<script type="text/javascript">

    $(document).ready( function () {
        $('#btn_mais').click( function () {
            $('#filtros').toggleClass('oculto');
        });

        $('#btn_config').click( function () {
            $('#config').toggleClass('oculto');
        });

        $('.multi').SumoSelect({
            search: true, 
            searchText: 'Digite para buscar', 
            placeholder: 'Filtrar pelos estados', 
            okCancelInMulti: true          
        });
    });

    // iniciando os nodos do grafo como os estados
    var nodes = new vis.DataSet(JSON.parse('<?= $estados; ?>'));

    // iniciando os arcos 
    var edges = new vis.DataSet(JSON.parse('<?= $acoes; ?>'));

    // iniciando container para o grafo
    var container = document.getElementById('mynetwork');

    // iniciando dados
    var data = {
        nodes: nodes,
        edges: edges
    };

    //configurações do grafo
    var options = {
            locale: "pt-br",
    		nodes: {
    			    shape: "box"
    		},
    		configure: {
    			  enabled: true, 
    			  filter: 'physics',   		
    			  showButton: false,
    			  container: document.getElementById("config")
    		},
    		interaction: {
    			hover: true,
    			keyboard: {
    			      enabled: true
    			},
    		    navigationButtons: true
    		},
    		physics: {
    		    forceAtlas2Based: {
    		      springLength: 100,
    		      avoidOverlap: 0.0
    		    },
    		    minVelocity: 0.75,
    		    solver: "forceAtlas2Based"
    		}
    }

/*
 * physics: {
	    enabled: true,
	    hierarchicalRepulsion: {
	        centralGravity: 0.0,
	        springLength: 500,
	        springConstant: 0.01,
	        nodeDistance: 400,
	        damping: 0.09
	    },
	    solver: 'hierarchicalRepulsion'
	}
 */
    
    // renderizando grafico
    var network = new vis.Network(container, data, options);

    //estabilizando engine de fisica para que os nodos não fiquem sacudindo
    network.stabilize();
</script>
