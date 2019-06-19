<?php
if ($_SERVER['HTTP_HOST'] == "simec-local" || $_SERVER['HTTP_HOST'] == "localhost") {
	define("WS_USUARIO_SIGEF", 'USAP_WS_SIGARP');
	define("WS_SENHA_SIGEF", '62856723');
} else {
	define("WS_USUARIO_SIGEF", 'USAP_WS_SIGARP');
	define("WS_SENHA_SIGEF", '62856723');
}

if($_SESSION['baselogin'] == 'simec_desenvolvimento') {
	
	# par3.obra_tipo_documento
	define('OBRA_TIPO_DOCUMENTO_PROPRIEDADE_TERRENO', 15);
	define('OBRA_TIPO_DOCUMENTO_PROPRIEDADE_IMOVEL', 20);
	
    # Mensagens de sistema
    define('MSG001', 'Operação realizada com sucesso!');
    define('MSG002', 'Operação não pode ser realizada!');

    # Workflow Estados
    define('PAR3_SIS_ID', 231);
    define('PAR3_WORKFLOW_FLUXO_DO_PAR_PREPARATORIA_DIAGNOSTIGO', 1638);
    define('PAR3_WORKFLOW_FLUXO_DO_PAR_DIAGNOSTIGO', 1637);
    define('PAR3_WORKFLOW_FLUXO_DO_PAR_ELABORACAO', 1683);
    define('PAR3_WORKFLOW_FLUXO_DO_PAR_ANALISE', 1874);
    define('PAR3_WORKFLOW_FLUXO_BANDA_LARGA', 290);
    //define('PAR3_WORKFLOW_FLUXO_OBRAS', 306); ANTIGO
    define('PAR3_WORKFLOW_FLUXO_OBRAS', 312);

    # FLUXO OBRA
    define('PAR3_ESDID_OBRA_EM_CADASTRAMENTO', 2007);
    
    # FLUXO UNIDADE
    define('PAR3_WORKFLOW_FLUXO_DO_PAR', 243);
    define('PAR3_AEDID_ENVIA_ANALISE', 5014);
    define('PAR3_ESDID_ELABORACAO', 1999);

    # new PAR 3
    define('PAR3_ESDID_ETAPA_PREPARATORIA_DO_DIAGNOSTICO', 1638);
    define('PAR3_ESDID_DIAGNOSTICO_ELABORACAO', 1637);
    define('PAR3_ESDID_DIAGNOSTICO_FINALIZADO', 1683);
    define('PAR3_ESDID_PLANEJAMENTO_NAO_INICIADO', 1874);
    define('PAR3_ESDID_PLANEJAMENTO_EM_ELABORACAO', 1999);

    # PAR3_WORKFLOW_FLUXO_DO_PAR_DIAGNOSTIGO

    # Workflow Ações
    define('PAR3_WORKFLOW_FINALIZAR_FASE_DE_ELABORACAO', 4539);
    define('PAR3_WORKFLOW_RETORNAR_FASE_DE_ELABORACAO', 4563);
    define('PAR3_WORKFLOW_FINALIZAR_FASE_DE_DIAGNOSTIGO', 4091);
    define('PAR3_AEDID_ENVIAR_PLANEJAMENTO_NAO_INICIADO', 4539);
    define('PAR3_WORKFLOW_FINALIZAR_FASE_PREPARATORIA_DIAGNOSTIGO', 3953);

    # Questionario  Questões Estratégicas
    define('QUESTIONARIO_QUESTOES_ESTRATEGICAS_EST', 5);
    define('QUESTIONARIO_QUESTOES_ESTRATEGICAS_MUN', 6);
    define('QUESTIONARIO_QUESTOES_ESTRATEGICAS_DF', 4);
    define('QUESTIONARIO_QUESTOES_PNE', 7);
    define('QUESTIONARIO_BANDA_LARGA', 133);

    # Questionarios - Obras
    define('QUESTIONARIO_OBRA_VISTORIA', 2);
    define('QUESTIONARIO_OBRA_VISTORIA2', 3);
    define('QUESTIONARIO_OBRA_ESTUDO_DEMANDA_CONSTRUCAO_INFANTIL', 11);
    define('QUESTIONARIO_OBRA_ESTUDO_DEMANDA_CONSTRUCAO_FUNDAMENTAL', 12);
    define('QUESTIONARIO_OBRA_ESTUDO_DEMANDA_CONSTRUCAO_MEDIO', 13);
    define('QUESTIONARIO_OBRA_ESTUDO_DEMANDA_AMPLIACAO_INFANTIL', 8);
    define('QUESTIONARIO_OBRA_ESTUDO_DEMANDA_AMPLIACAO_FUNDAMENTAL', 9);
    define('QUESTIONARIO_OBRA_ESTUDO_DEMANDA_AMPLIACAO_MEDIO', 10);

    # TIPOS Fluxos dos programas ABCMAIS E PROEMI
    define('WF_TPDID_ABCMAIS', 271);
    define('WF_TPDID_MAISALFABETIZACAO', 315);
    define('WF_TPDID_ESCOLAACESSIVEL', 329);
    define('WF_TPDID_NOVO_ENSINO_MEDIO', 356);
    define('WF_TPDID_PROEMI', 272);
    # ESTADO WF NOVO ENSINO MEDIO
    define('WF_ESDID_NAO_INICIADO_PNEM', 2376);
    define('WF_ESDID_EMCADASTRAMENTO_PNEM', 2377);
    define('WF_ESDID_ENVIADOAOMEC_PNEM', 2378);
    # ESTADO WF ABCMAIS
    define('WF_ESDID_NAO_INICIADO_ABCMAIS', 1810);
    define('WF_ESDID_EMCADASTRAMENTO_ABCMAIS', 1812);
    define('WF_ESDID_ENVIADOPARAOMEC_ABCMAIS', 1815);
    define('WF_ESDID_TERMONAOACEITO_ABCMAIS', 1817);
    # ESTADO WF MAISALFABETIZACAO
    define('WF_ESDID_NAO_INICIADO_MAISALFABETIZACAO', 2097);
    define('WF_ESDID_EMCADASTRAMENTO_MAISALFABETIZACAO', 2098);
    define('WF_ESDID_ENVIADOPARAOMEC_MAISALFABETIZACAO', 2099);
    define('WF_ESDID_TERMONAOACEITO_MAISALFABETIZACAO', 2100);
    # ESTADO WF ESCOLAACESSIVEL
    define('WF_ESDID_NAO_INICIADO_ESCOLAACESSIVEL', 2178);
    define('WF_ESDID_EMCADASTRAMENTO_ESCOLAACESSIVEL', 2179);
    define('WF_ESDID_ENVIADOPARAOMEC_ESCOLAACESSIVEL', 2180);
    define('WF_ESDID_TERMONAOACEITO_ESCOLAACESSIVEL', 2181);
    # ESTADO WF PROEMI
    define('WF_ESDID_NAO_INICIADO_PROEMI', 1811);
    define('WF_ESDID_EMCADASTRAMENTO_PROEMI', 1813);
    define('WF_ESDID_ENVIADOPARAOMEC_PROEMI', 1814);
    define('WF_ESDID_TERMONAOACEITO_PROEMI', 1816);
    # Tramitacoes Banda Larga
    define('WF_ESDID_INICIADO', 1913);
    define('WF_ESDID_FIANLIZADO', 1914);
    # Tramitacoes Banda Larga
    define('WF_AEDID_FINALIZAR', 4618);
    define('WF_AEDID_REABRIR', 4619);
    # Tramitacoes Novo Ensino Medio
    define('WF_AEDID_PNEM_NAOINICIADO_CADASTRAMENTO', 5658);
    define('WF_AEDID_PNEM_CADASTRAMENTO_ENVIADO', 5659);
    # Tramitacoes ABCMAIS
    define('WF_AEDID_ABCMAIS_NAOINICIADO_CADASTRAMENTO', 4389);
    define('WF_AEDID_ABCMAIS_NAOINICIADO_NAOACEITO', 4390);
    define('WF_AEDID_ABCMAIS_CADASTRAMENTO_ENVIADOPARAMEC', 4393);
    define('WF_AEDID_ABCMAIS_CADASTRAMENTO_TERMONAOACEITO', 4399);
    define('WF_AEDID_ABCMAIS_ENVIADOPARAMEC_EMCADASTRAMENTO', 4395);
    define('WF_AEDID_ABCMAIS_TERMONAOACEITO_EMCADASTRAMENTO', 4397);
    # Tramitacoes MAISALFABETIZACAO
    define('WF_AEDID_MAISALFABETIZACAO_NAOINICIADO_CADASTRAMENTO', 5211);
    define('WF_AEDID_MAISALFABETIZACAO_NAOINICIADO_NAOACEITO', 5212);
    define('WF_AEDID_MAISALFABETIZACAO_CADASTRAMENTO_ENVIADOPARAMEC', 5214);
    define('WF_AEDID_MAISALFABETIZACAO_CADASTRAMENTO_TERMONAOACEITO', 5213);
    define('WF_AEDID_MAISALFABETIZACAO_ENVIADOPARAMEC_EMCADASTRAMENTO', 5215);
    define('WF_AEDID_MAISALFABETIZACAO_TERMONAOACEITO_EMCADASTRAMENTO', 5216);
    # Tramitacoes ESCOLAACESSIVEL
    define('WF_AEDID_ESCOLAACESSIVEL_NAOINICIADO_CADASTRAMENTO', 5420);
    define('WF_AEDID_ESCOLAACESSIVEL_NAOINICIADO_NAOACEITO', 5421);
    define('WF_AEDID_ESCOLAACESSIVEL_CADASTRAMENTO_ENVIADOPARAMEC', 5423);
    define('WF_AEDID_ESCOLAACESSIVEL_CADASTRAMENTO_TERMONAOACEITO', 5422);
    define('WF_AEDID_ESCOLAACESSIVEL_ENVIADOPARAMEC_EMCADASTRAMENTO', 5424);
    define('WF_AEDID_ESCOLAACESSIVEL_TERMONAOACEITO_EMCADASTRAMENTO', 5425);
    # Tramitacoes PROEMI
    define('WF_AEDID_PROEMI_NAOINICIADO_CADASTRAMENTO', 4391);
    define('WF_AEDID_PROEMI_NAOINICIADO_NAOACEITO', 4392);
    define('WF_AEDID_PROEMI_CADASTRAMENTO_TERMONAOACEITO', 4400);
    define('WF_AEDID_PROEMI_CADASTRAMENTO_ENVIADOPARAMEC', 4394);
    define('WF_AEDID_PROEMI_ENVIADOPARAMEC_EMCADASTRAMENTO', 4396);
    define('WF_AEDID_PROEMI_TERMONAOACEITO_EMCADASTRAMENTO', 4398);

    # ID para inserir observação nos programas PROEMI E ABC MAIS
    define('ORIENTACOES_PROEMI_ABA_ESCOLAS', 1);
    define('ORIENTACOES_PROEMI_ABA_INTEGRACAO_CURRICULAR', 2);
    define('ORIENTACOES_PROEMI_EXCESAO_ESCOLAS', 6);
    define('ORIENTACOES_ABCMAIS_ABA_ESCOLAS', 3);
    define('ORIENTACOES_ABCMAIS_ABA_ESCOLAS_2016', 3);
    define('ORIENTACOES_ABCMAIS_ABA_ESCOLAS_2017', 7);
    define('ORIENTACOES_ABCMAIS_EXCESAO_ESCOLAS', 5);
    define('ORIENTACOES_ABCMAIS_ABA_COORDENADOR', 4);

    define('ORIENTACOES_MAISALFABETIZACAO_ABA_ESCOLAS', 8);
    define('ORIENTACOES_MAISALFABETIZACAO_EXCESAO_ESCOLAS', 9);
    define('ORIENTACOES_MAISALFABETIZACAO_ABA_COORDENADOR', 10);
    
    # orientações escola acessível
    define('ORIENTACOES_ESCOLAACESSIVEL_ABA_ESCOLAS', 11);
    
    # orientações Novo Ensino Médio
    define('ORIENTACOES_NEM_ABA_ESCOLAS', 12);
    
    # Programas
    define('PRGID_PROEMI', 9);
    define('PRGID_ABCMAIS', 8);
    define('PRGID_MAIS_ALFABETIZACAO', 51);
    define('PRGID_ESCOLA_ACESSIVEL', 55);
    define('PRGID_NOVO_ENSINO_MEDIO', 56);
    define('PRGID_APOIO_EMERGENCIAL_MUNICIPIOS', 54);


    define('TENID_PREFEITO', 2);
    define('TENID_DIRIGENTE', 4);
    define('TENID_SECRETARIO_ESTADUAL', 9);

    define('SISID_PAR3', 231);
    define('SISID_OBRAS2', 147);
    define('SISID_PAR', 23);

    define('PAR3_PERFIL_EQUIPE_ESTADUAL_SECRETARIO', 1437);
    define('PAR3_PERFIL_DIRIGENTE_MUNICIPAL', 1433);

    define('PAR3_PERFIL_SUPER_USUARIO', 1430);
    define('PAR3_PERFIL_ADMINISTRADOR', 1445);
    define('PAR3_PERFIL_PREFEITO', 1436);
    define('PAR3_PERFIL_DIRIGENTE_MUNICIPAL_DE_EDUCACAO', 1433);
    define('PAR3_PERFIL_EQUIPE_MUNICIPAL', 1431);
    define('PAR3_PERFIL_SECRETARIO_ESTADUAL', 1437);
    define('PAR3_PERFIL_EQUIPE_ESTADUAL', 1432);

    define('PAR3_PERFIL_CONTROLE_SOCIAL_PRESIDENTE', 1501);
    define('PAR3_PERFIL_CONSULTA_DE_USUARIO', 1492);
    define('PAR3_PERFIL_CONTROLE_SOCIAL_CONSELHEIRO', 1502);
    define('PAR3_PERFIL_ANALISTA_DO_CAE', 1519);
    define('PAR3_PERFIL_SUPORTE_MEC', 1468);
    define('PAR3_PERFIL_ADMINISTRADOR_ACOES_NOVO_PAR', 1539);
    define('PAR3_PERFIL_NUTRICIONISTA', 1435);
    define('PAR3_PERFIL_CONSULTA_GERAL', 1447);
    define('PAR3_PERFIL_CONTROLE_DE_USUARIO', 1461);
    define('PAR3_PERFIL_SUPORTE_FNDE', 1469);
    define('PAR3_PERFIL_CONSULTA_ESTADUAL', 1480);
    define('PAR3_PERFIL_CONSELHEIRO', 1477);
    define('PAR3_PERFIL_ANALISTA_DE_PROGRAMAS', 1479);
    define('PAR3_PERFIL_CONSULTA_MUNICIPAL', 1481);
    define('PAR3_PERFIL_ANALISTA_PROFE', 1693);
    define('PAR3_PERFIL_PROCURADOR_PROFE', 1692);
    define('PAR3_PERFIL_ANALISTA_CGEST', 1691);
    define('PAR3_PERFIL_COORDENADOR_CGEST', 1690);
    define('PAR3_PERFIL_PARLAMENTAR', 1775);

    define('OBRAS2_GESTOR_UNIDADE', 946);
    define('PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO', 674);

    # Educacenso
    define('SCHEMAEDUCACENSO', 'educacenso_2014');

    # Perfil Adm_acoes
    define('PFL_ADM_ACOES', '1539');

    # Adesao de cursos de medicina
    define('DUTID_PREFEITURA', 6);
    define('DUTID_PREFEITO', 7);

    # DESDOBRAMENTOS
    //define('DESDOBRAMENTO_CAMPO', 10);
    define('DESDOBRAMENTO_URBANO', 11);
    define('DESDOBRAMENTO_INTEGRAL', 12);
    define('DESDOBRAMENTO_PARCIAL', 13);
    define('DESDO3BRAMENTO_RURAL', 14);


    #FLUXO  Fluxo PAR 3 Programa Apoio Emergencial aos Municípios do FPM
    define('PAR3_FLUXO_FPM', 337);

     #Fluxo do Habilita
     define('PAR3_FLUXO_HABILITA', 353);
     define('PAR3_ESD_HABILITA_EM_CADASTRAMENTO', 2354);
     define('PAR3_ESD_HABILITA_AGUARDANDO_DISTRIBUICAO', 2355);
     define('PAR3_ESD_HABILITA_AGUARDANDO_ANALISE', 2380);
     define('PAR3_ESD_HABILITA_EM_ANALISE', 2356);
     define('PAR3_ESD_HABILITA_EM_DILIGENCIA', 2357);
     define('PAR3_ESD_HABILITA_HABILITADO', 2358);
     
     define('PAR3_AEDID_HABILITA_ENVIA_ANALISE', 5585);
     define('WF_AEDID_HABILITA_AGUARDANDODISTRIBUICAO_EMANALISE', 5586);
     define('PAR3_AEDID_HABILITA_ENVIA_DILIGENCIA', 5587);
     define('PAR3_AEDID_HABILITA_ENVIA_DILIGENCIA_ANALISE', 5589);
     define('PAR3_AEDID_HABILITA_ENVIA_ENTIDADE_HABILITADA', 5588);
    

} else {
	# par3.obra_tipo_documento
	define('OBRA_TIPO_DOCUMENTO_PROPRIEDADE_TERRENO', 15);
	define('OBRA_TIPO_DOCUMENTO_PROPRIEDADE_IMOVEL', 20);
	
    # Mensagens de sistema
    define('MSG001', 'Operação realizada com sucesso!');
    define('MSG002', 'Operação não pode ser realizada!');

    define('CPF_PRESIDENTE_FNDE', '38826658404');
    # Workflow Estados
    define('PAR3_SIS_ID', 231);
    define('PAR3_WORKFLOW_FLUXO_DO_PAR_PREPARATORIA_DIAGNOSTIGO', 1638);
    define('PAR3_WORKFLOW_FLUXO_DO_PAR_DIAGNOSTIGO', 1637);
    define('PAR3_WORKFLOW_FLUXO_DO_PAR_ELABORACAO', 1683);
    define('PAR3_WORKFLOW_FLUXO_DO_PAR_ANALISE', 1874);
    define('PAR3_WORKFLOW_FLUXO_BANDA_LARGA', 290);
    define('PAR3_WORKFLOW_FLUXO_OBRAS', 304);
        
    #FLUXO DE PRORROGAÇÃO DE TERMO - DE OFÍCIO
    define('PAR3_ESDID_OFICIO_CADASTRAMENTO', 2445);
    define('PAR3_ESDID_OFICIO_AGUARDANDO_APROVACAO_COORDENADOR', 2446);
    define('PAR3_ESDID_OFICIO_APROVADA', 2447);
    define('PAR3_ESDID_OFICIO_CANCELADA', 2448);
    
    
    # FLUXO OBRA
    define('PAR3_ESDID_OBRA_EM_CADASTRAMENTO', 2007);
    
    # FLUXO UNIDADE
    define('PAR3_WORKFLOW_FLUXO_DO_PAR', 243);
    define('PAR3_AEDID_ENVIA_ANALISE', 4948);
    define('PAR3_ESDID_ELABORACAO', 1999);

    # new
    define('PAR3_ESDID_ETAPA_PREPARATORIA', 1638);
    define('PAR3_ESDID_ETAPA_PREPARATORIA_DO_DIAGNOSTICO', 1638);
    define('PAR3_ESDID_DIAGNOSTICO_ELABORACAO', 1637);
    define('PAR3_ESDID_DIAGNOSTICO_FINALIZADO', 1683);
    define('PAR3_ESDID_PLANEJAMENTO_NAO_INICIADO', 1874);
    define('PAR3_ESDID_PLANEJAMENTO_EM_ELABORACAO', 1999);
    define('PAR3_ESDID_PLANEJAMENTO_EFINALIZADO', 2000);

    # FLUXO INICIATIVAS
    define('PAR3_WORKFLOW_FLUXO_INICIATIVAS', 302);
    define('PAR3_AEDID_ANALISE_MERITO', 5137);
    define('PAR3_AEDID_CANCELADA', 5135);
    //define('PAR3_AEDID_AGUARDANDO_ANALISE', 5136);
    define('PAR3_ESDID_CADASTRAMENTO', 1990);
    define('PAR3_AEDID_AGUARDANDO_ANALISE', 5164);
    define('PAR3_ESDID_EM_DILIGENCIA', 2078);
    define('PAR3_ESDID_AGUARDANDO_ANALISE', 2077);
    define('PAR3_ESDID_EM_ANALISE', 2069);
    define('PAR3_ESDID_EM_ANALISE_CANCELADA', 2076);

    define('PAR3_WORKFLOW_FLUXO_DO_PAR_CADASTRAMENTO', 1720);
    define('PAR3_WORKFLOW_FLUXO_DO_PAR_ANALISE_MEC', 1721);
    define('PAR3_WORKFLOW_FLUXO_DO_PAR_DILIGENCIA', 1722);

    # FLUXO DE ANÁLISE
    define('PAR3_ESDID_PLANEJAMENTO_AGUARDANDO_ANALISE', 2070);
    define('PAR3_ESDID_PLANEJAMENTO_ANALISE', 2071);
    define('PAR3_ESDID_AGUARDANDO_VALIDACAO_COORDENADOR', 2072);
    define('PAR3_ESDID_EM_VALIDACAO_COORDENADOR', 2073);
    define('PAR3_ESDID_ANALISE_DILIGENCIA', 2074);
    define('PAR3_ESDID_ANALISE_PLANEJAMENTO_APROVADO', 2075);
    
    # FLUXO DE REFORMULAÇÃO INICIATIVA
    define('PAR3_REFORMULACAO_INICIATIVA', 2);    
    define('PAR3_REFORMULACAO_ESDID_EM_CADASTRAMENTO', 2363);
    define('PAR3_REFORMULACAO_ESDID_AGUARDANDO_ANALISE', 2364);
    define('PAR3_REFORMULACAO_ESDID_EM_ANALISE', 2365);
    define('PAR3_REFORMULACAO_ESDID_EM_DILIGENCIA', 2366);
    define('PAR3_REFORMULACAO_ESDID_EM_ANALISE_RETORNO_DILIGENCIA', 2367);
    define('PAR3_REFORMULACAO_ESDID_EM_VALIDACAO_COORDENADOR', 2368);
    define('PAR3_REFORMULACAO_ESDID_REFORMULACAO_APROVADA', 2369);
    define('PAR3_REFORMULACAO_ESDID_AGUARDANDO_GERACAO_TERMO', 2370);
    define('PAR3_REFORMULACAO_ESDID_REPROGRAMACAO_FINALIZADA', 2371);
    define('PAR3_REFORMULACAO_ESDID_AGUARDANDO_COMPLEMENTACAO_EMPENHO', 2372);
    define('PAR3_REFORMULACAO_ESDID_REFORMULACAO_REPROVADA', 2373);
    define('PAR3_REFORMULACAO_ESDID_REFORMULACAO_CANCELADA', 2374);
    define('PAR3_REFORMULACAO_ESDID_AGUARDANDO_VALIDACAO_GESTOR', 2375);
    
    # FLUXO DE REFORMULAÇÃO PRAZO
    define('PAR3_REFORMULACAO_PRAZO', 1);
    define('PAR3_REFORMULACAO_PRAZO_ESDID_EM_CADASTRAMENTO', 2432);
    define('PAR3_REFORMULACAO_PRAZO_ESDID_EM_AGUARDANDO_ANALISE', 2341);
    define('PAR3_REFORMULACAO_PRAZO_ESDID_EM_ANALISE', 2457);
    define('PAR3_REFORMULACAO_PRAZO_ESDID_EM_ANALISE_CGEST', 2452);
    define('PAR3_REFORMULACAO_PRAZO_ESDID_EM_ANALISE_CGIMP', 2453);
    define('PAR3_REFORMULACAO_PRAZO_ESDID_EM_ANALISE_CGPES', 2454);
    define('PAR3_REFORMULACAO_PRAZO_ESDID_EM_DILIGENCIA', 2455);
    define('PAR3_REFORMULACAO_PRAZO_ESDID_EM_CANCELADA', 2456);    
    define('PAR3_REFORMULACAO_PRAZO_ESDID_EM_AGUARDANDO_VALIDACAO_COORDENADOR', 2458);
    define('PAR3_REFORMULACAO_PRAZO_ESDID_EM_PRAZO_APROVADA', 2459);
    define('PAR3_REFORMULACAO_PRAZO_ESDID_EM_GERACAO_TERMO', 2460);
    define('PAR3_REFORMULACAO_PRAZO_ESDID_EM_VALIDACAO_GESTOR', 2461);
    define('PAR3_REFORMULACAO_PRAZO_ESDID_EM_FINALIZADA', 2462);
    define('PAR3_REFORMULACAO_PRAZO_ESDID_EM_REPROVADA', 2463);
    define('PAR3_REFORMULACAO_PRAZO_ESDID_EM_PRORROGACAO_OFICIO', 2477);
    
    define('PAR3_REFORMULACAO_PRAZO_ESDID_EM_RECURSO_AGUARDANDO_ANALISE', 2464);
    define('PAR3_REFORMULACAO_PRAZO_ESDID_EM_RECURSO_ANALISE', 2465);
    define('PAR3_REFORMULACAO_PRAZO_ESDID_EM_RECURSO_ANALISE_CGEST', 2466);
    define('PAR3_REFORMULACAO_PRAZO_ESDID_EM_RECURSO_ANALISE_CGIMP', 2467);
    define('PAR3_REFORMULACAO_PRAZO_ESDID_EM_RECURSO_ANALISE_CGPES', 2468);
    define('PAR3_REFORMULACAO_PRAZO_ESDID_EM_RECURSO_DILIGENCIA', 2469);
    define('PAR3_REFORMULACAO_PRAZO_ESDID_EM_RECURSO_AGUARDANDO APROVACAO_COORDENADOR', 2470);
    define('PAR3_REFORMULACAO_PRAZO_ESDID_EM_RECURSO_ANALISE_DIGAP', 2471);
    define('PAR3_REFORMULACAO_PRAZO_ESDID_EM_FINALIZADA_REPROVADA', 2472);
    define('PAR3_REFORMULACAO_PRAZO_ESDID_EM_RECURSO_APROVADA', 2473);    
    
    # Ações Análise Iniciativas
    define('ANALISE_AEDID_VALIDACAO_COORDENADOR_PLANEJAMENTO_INICIATIVA', 5138);
    # PAR3_WORKFLOW_FLUXO_DO_PAR_DIAGNOSTIGO

    # Workflow Ações
    define('PAR3_WORKFLOW_FINALIZAR_FASE_DE_ELABORACAO', 4539);
    define('PAR3_WORKFLOW_RETORNAR_FASE_DE_ELABORACAO', 4563);
    define('PAR3_WORKFLOW_FINALIZAR_FASE_DE_DIAGNOSTIGO', 4091);
    define('PAR3_AEDID_ENVIAR_PLANEJAMENTO_NAO_INICIADO', 4539);
    define('PAR3_WORKFLOW_FINALIZAR_FASE_PREPARATORIA_DIAGNOSTIGO', 3953);

    # Questionario  Questões Estratégicas
    define('QUESTIONARIO_QUESTOES_ESTRATEGICAS_EST', 129);
    define('QUESTIONARIO_QUESTOES_ESTRATEGICAS_MUN', 130);
    define('QUESTIONARIO_QUESTOES_ESTRATEGICAS_DF', 128);
    define('QUESTIONARIO_QUESTOES_PNE', 131);
    define('QUESTIONARIO_BANDA_LARGA', 133);

    # Questionarios - Obras
    define('QUESTIONARIO_OBRA_VISTORIA', 137);
    define('QUESTIONARIO_OBRA_VISTORIA2', 138);
    define('QUESTIONARIO_OBRA_ESTUDO_DEMANDA_CONSTRUCAO_INFANTIL', 140);
    define('QUESTIONARIO_OBRA_ESTUDO_DEMANDA_CONSTRUCAO_FUNDAMENTAL', 140);
    define('QUESTIONARIO_OBRA_ESTUDO_DEMANDA_CONSTRUCAO_MEDIO', 140);
    define('QUESTIONARIO_OBRA_ESTUDO_DEMANDA_AMPLIACAO_INFANTIL', 140);
    define('QUESTIONARIO_OBRA_ESTUDO_DEMANDA_AMPLIACAO_FUNDAMENTAL', 140);
    define('QUESTIONARIO_OBRA_ESTUDO_DEMANDA_AMPLIACAO_MEDIO', 140);

    # TIPOS Fluxos dos programas ABCMAIS E PROEMI
    define('WF_TPDID_ABCMAIS', 271);
    define('WF_TPDID_MAISALFABETIZACAO', 315);
    define('WF_TPDID_ESCOLAACESSIVEL', 329);
    define('WF_TPDID_NOVO_ENSINO_MEDIO', 356);
    define('WF_TPDID_FPM', 337);
    define('WF_TPDID_PROEMI', 272);
    # ESTADO WF NOVO ENSINO MEDIO
    define('WF_ESDID_NAO_INICIADO_PNEM', 2376);
    define('WF_ESDID_EMCADASTRAMENTO_PNEM', 2377);
    define('WF_ESDID_ENVIADOAOMEC_PNEM', 2378);
    # ESTADO WF ABCMAIS
    define('WF_ESDID_NAO_INICIADO_ABCMAIS', 1810);
    define('WF_ESDID_EMCADASTRAMENTO_ABCMAIS', 1812);
    define('WF_ESDID_ENVIADOPARAOMEC_ABCMAIS', 1815);
    define('WF_ESDID_TERMONAOACEITO_ABCMAIS', 1817);
    # ESTADO WF MAISALFABETIZACAO
    define('WF_ESDID_NAO_INICIADO_MAISALFABETIZACAO', 2097);
    define('WF_ESDID_EMCADASTRAMENTO_MAISALFABETIZACAO', 2098);
    define('WF_ESDID_ENVIADOPARAOMEC_MAISALFABETIZACAO', 2099);
    define('WF_ESDID_TERMONAOACEITO_MAISALFABETIZACAO', 2100);
    # ESTADO WF ESCOLAACESSIVEL
    define('WF_ESDID_NAO_INICIADO_ESCOLAACESSIVEL', 2178);
    define('WF_ESDID_EMCADASTRAMENTO_ESCOLAACESSIVEL', 2179);
    define('WF_ESDID_ENVIADOPARAOMEC_ESCOLAACESSIVEL', 2180);
    define('WF_ESDID_TERMONAOACEITO_ESCOLAACESSIVEL', 2181);
    # ESTADO WF PROEMI
    define('WF_ESDID_NAO_INICIADO_PROEMI', 1811);
    define('WF_ESDID_EMCADASTRAMENTO_PROEMI', 1813);
    define('WF_ESDID_ENVIADOPARAOMEC_PROEMI', 1814);
    define('WF_ESDID_TERMONAOACEITO_PROEMI', 1816);
    # Tramitacoes Banda Larga
    define('WF_ESDID_INICIADO', 1913);
    define('WF_ESDID_FIANLIZADO', 1914);
    # Tramitacoes Banda Larga
    define('WF_AEDID_FINALIZAR', 4618);
    define('WF_AEDID_REABRIR', 4619);
    # Tramitacoes Novo Ensino Medio
    define('WF_AEDID_PNEM_NAOINICIADO_CADASTRAMENTO', 5658);
    define('WF_AEDID_PNEM_CADASTRAMENTO_ENVIADO', 5659);
    # Tramitacoes ABCMAIS
    define('WF_AEDID_ABCMAIS_NAOINICIADO_CADASTRAMENTO', 4389);
    define('WF_AEDID_ABCMAIS_NAOINICIADO_NAOACEITO', 4390);
    define('WF_AEDID_ABCMAIS_CADASTRAMENTO_ENVIADOPARAMEC', 4393);
    define('WF_AEDID_ABCMAIS_CADASTRAMENTO_TERMONAOACEITO', 4399);
    define('WF_AEDID_ABCMAIS_ENVIADOPARAMEC_EMCADASTRAMENTO', 4395);
    define('WF_AEDID_ABCMAIS_TERMONAOACEITO_EMCADASTRAMENTO', 4397);
    # Tramitacoes MAISALFABETIZACAO
    define('WF_AEDID_MAISALFABETIZACAO_NAOINICIADO_CADASTRAMENTO', 5211);
    define('WF_AEDID_MAISALFABETIZACAO_NAOINICIADO_NAOACEITO', 5212);
    define('WF_AEDID_MAISALFABETIZACAO_CADASTRAMENTO_ENVIADOPARAMEC', 5214);
    define('WF_AEDID_MAISALFABETIZACAO_CADASTRAMENTO_TERMONAOACEITO', 5213);
    define('WF_AEDID_MAISALFABETIZACAO_ENVIADOPARAMEC_EMCADASTRAMENTO', 5215);
    define('WF_AEDID_MAISALFABETIZACAO_TERMONAOACEITO_EMCADASTRAMENTO', 5216);
    # Tramitacoes ESCOLAACESSIVEL
    define('WF_AEDID_ESCOLAACESSIVEL_NAOINICIADO_CADASTRAMENTO', 5420);
    define('WF_AEDID_ESCOLAACESSIVEL_NAOINICIADO_NAOACEITO', 5421);
    define('WF_AEDID_ESCOLAACESSIVEL_CADASTRAMENTO_ENVIADOPARAMEC', 5423);
    define('WF_AEDID_ESCOLAACESSIVEL_CADASTRAMENTO_TERMONAOACEITO', 5422);
    define('WF_AEDID_ESCOLAACESSIVEL_ENVIADOPARAMEC_EMCADASTRAMENTO', 5424);
    define('WF_AEDID_ESCOLAACESSIVEL_TERMONAOACEITO_EMCADASTRAMENTO', 5425);
    # Tramitacoes PROEMI
    define('WF_AEDID_PROEMI_NAOINICIADO_CADASTRAMENTO', 4391);
    define('WF_AEDID_PROEMI_NAOINICIADO_NAOACEITO', 4392);
    define('WF_AEDID_PROEMI_CADASTRAMENTO_TERMONAOACEITO', 4400);
    define('WF_AEDID_PROEMI_CADASTRAMENTO_ENVIADOPARAMEC', 4394);
    define('WF_AEDID_PROEMI_ENVIADOPARAMEC_EMCADASTRAMENTO', 4396);
    define('WF_AEDID_PROEMI_TERMONAOACEITO_EMCADASTRAMENTO', 4398);

    # ID para inserir observação nos programas PROEMI E ABC MAIS
    define('ORIENTACOES_PROEMI_ABA_ESCOLAS', 1);
    define('ORIENTACOES_PROEMI_ABA_INTEGRACAO_CURRICULAR', 2);
    define('ORIENTACOES_ABCMAIS_ABA_ESCOLAS', 3);
    define('ORIENTACOES_ABCMAIS_ABA_ESCOLAS_2016', 3);
    define('ORIENTACOES_ABCMAIS_ABA_ESCOLAS_2017', 7);
    define('ORIENTACOES_PROEMI_EXCESAO_ESCOLAS', 6);
    define('ORIENTACOES_ABCMAIS_EXCESAO_ESCOLAS', 5);
    define('ORIENTACOES_ABCMAIS_ABA_COORDENADOR', 4);

    define('ORIENTACOES_MAISALFABETIZACAO_ABA_ESCOLAS', 8);
    define('ORIENTACOES_MAISALFABETIZACAO_EXCESAO_ESCOLAS', 9);
    define('ORIENTACOES_MAISALFABETIZACAO_ABA_COORDENADOR', 10);
    
    # orientações escola acessível
    define('ORIENTACOES_ESCOLAACESSIVEL_ABA_ESCOLAS', 11);
    
    # orientações Novo Ensino Médio
    define('ORIENTACOES_NEM_ABA_ESCOLAS', 12);
    
    # Programas
    define('PRGID_PROEMI', 9);
    define('PRGID_ABCMAIS', 8);
    define('PRGID_MAIS_ALFABETIZACAO', 51);
    define('PRGID_ESCOLA_ACESSIVEL', 55);
    define('PRGID_NOVO_ENSINO_MEDIO', 56);
    define('PRGID_APOIO_EMERGENCIAL_MUNICIPIOS', 54);

    define('TENID_PREFEITURA', 1);
    define('TENID_SECRETARIA_EDUCACAO', 3);
    define('TENID_PREFEITO', 2);
    define('TENID_DIRIGENTE', 4);
    define('TENID_SECRETARIO_ESTADUAL', 9);

    define('SISID_PAR3', 231);
    define('SISID_OBRAS2', 147);
    define('SISID_PAR', 23);

    define('PAR3_PERFIL_EQUIPE_ESTADUAL_SECRETARIO', 1437);
    define('PAR3_PERFIL_DIRIGENTE_MUNICIPAL', 1433);

    define('PAR3_PERFIL_SUPER_USUARIO_DESENVOLVEDOR', 1756);
    define('PAR3_PERFIL_SUPER_USUARIO', 1430);
    define('PAR3_PERFIL_ADMINISTRADOR', 1445);
    define('PAR3_PERFIL_PREFEITO', 1436);
    define('PAR3_PERFIL_DIRIGENTE_MUNICIPAL_DE_EDUCACAO', 1433);
    define('PAR3_PERFIL_EQUIPE_MUNICIPAL', 1431);
    define('PAR3_PERFIL_CURSO_MEDICINA_AVALIADOR_INST', 1653);
    define('PAR3_PERFIL_SECRETARIO_ESTADUAL', 1437);
    define('PAR3_PERFIL_SECRETARIO_MUNICIPAL', 1694);
    define('PAR3_PERFIL_EQUIPE_ESTADUAL', 1432);

    define('PAR3_PERFIL_CONTROLE_SOCIAL_PRESIDENTE', 1501);
    define('PAR3_PERFIL_CONSULTA_DE_USUARIO', 1492);
    define('PAR3_PERFIL_CONTROLE_SOCIAL_CONSELHEIRO', 1502);
    define('PAR3_PERFIL_ANALISTA_DO_CAE', 1519);
    define('PAR3_PERFIL_SUPORTE_MEC', 1468);
    define('PAR3_PERFIL_ADMINISTRADOR_ACOES_NOVO_PAR', 1539);
    define('PAR3_PERFIL_NUTRICIONISTA', 1435);
    define('PAR3_PERFIL_CONSULTA_GERAL', 1447);
    define('PAR3_PERFIL_CONTROLE_DE_USUARIO', 1461);
    define('PAR3_PERFIL_SUPORTE_FNDE', 1469);
    define('PAR3_PERFIL_CONSULTA_ESTADUAL', 1480);
    define('PAR3_PERFIL_CONSELHEIRO', 1477);
    define('PAR3_PERFIL_ANALISTA_DE_PROGRAMAS', 1479);
    define('PAR3_PERFIL_CONSULTA_MUNICIPAL', 1481);
    define('PAR3_PERFIL_ANALISTA_PROFE', 1693);
    define('PAR3_PERFIL_PROCURADOR_PROFE', 1692);
    define('PAR3_PERFIL_ANALISTA_CGEST', 1691);
    define('PAR3_PERFIL_COORDENADOR_CGEST', 1690);
    define('PAR3_PERFIL_PARLAMENTAR', 1775);
    define('PAR3_PERFIL_ANALISTA_PLANEJAMENTO', 1771);
    define('PAR3_PERFIL_ENTIDADE_EXECUTORA', 1862);

    define('OBRAS2_GESTOR_UNIDADE', 946);
    define('PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO', 674);

    # Educacenso
    define('SCHEMAEDUCACENSO', 'educacenso_2014');

    # Perfil Adm_acoes
    define('PFL_ADM_ACOES', '1539');

    # Adesao de cursos de medicina
    define('DUTID_PREFEITURA', 6);
    define('DUTID_PREFEITO', 7);

    # DESDOBRAMENTOS
    //define('DESDOBRAMENTO_CAMPO', 10);
    define('DESDOBRAMENTO_URBANO', 1);
    define('DESDOBRAMENTO_INTEGRAL', 4);
    define('DESDOBRAMENTO_PARCIAL', 3);
    define('DESDOBRAMENTO_RURAL', 2);

    #Fluxo de Validação de Documentos
    define('PAR3_FLUXO_VALIDACAO_DOCUMENTOS', 316);
    define('PAR3_ESD_DOCUMENTO_AGUARDANDO_ANALISE', 2106);
    define('PAR3_ESD_DOCUMENTO_EMANALISE', 2107);
    define('PAR3_ESD_DOCUMENTO_EMDILIGENCIA', 2108);
    define('PAR3_ESD_DOCUMENTO_VALIDACAO_COORDENADOR', 2149);
    define('PAR3_ESD_DOCUMENTO_APROVADO', 2111);
    define('PAR3_ESD_DOCUMENTO_REPROVADO', 2112);
    define('PAR3_ESD_DOCUMENTO_CANCELADO', 2113);
    
    #Fluxo de EMENDAS
    define('PAR3_FLUXO_EMENDA', 317);
    define('PAR3_ESD_EMENDA_EM_CADASTRAMENTO', 2115);
    define('PAR3_ESD_EMENDA_ANALISE_ENTIDADE', 2116);
    define('PAR3_ESD_EMENDA_ACEITA', 2117);
    define('PAR3_ESD_EMENDA_REJEITADA', 2118);
    define('PAR3_ESD_EMENDA_AJUSTE_PARLAMENTAR', 2146);

    #Fluxo SOLICITAÇÃO DESEMBOLSO
    define('PAR3_FLUXO_SOLICITACAO_DESEMBOLSO', 327);
    define('PAR3_DESEMBOLSO_AGUARDANDOANALISE', 2173);
    define('PAR3_DESEMBOLSO_EMANALISE', 2172);
    define('PAR3_DESEMBOLSO_DILIGENCIA', 2171);
    define('PAR3_DESEMBOLSO_VALIDACAO_COORDENADOR', 2167);
    define('PAR3_DESEMBOLSO_APROVADO', 2170);
    define('PAR3_DESEMBOLSO_REPROVADO', 2169);
    define('PAR3_DESEMBOLSO_CANCELADO', 2168);

    #FLUXO PAR3 PROGRAMA "Apoio emergencial aos municípios do FPM"
    define('PAR3_FLUXO_FPM_NAO_INICIADO', 2239);        //O Secretário Municipal acessou a adesão mas não executou nenhuma ação.
    define('PAR3_FLUXO_FPM_EM_CADASTRAMENTO', 2240);    //O Secretário Municipal aceitou o termo.
    define('PAR3_FLUXO_FPM_NAO_ACEITOU_TERMO', 2241);   //O Secretário Municipal não aceitou o termo.
    define('PAR3_FLUXO_FPM_ENVIADO_AO_MEC', 2242);      //O Secretário Municipal enviou adesão ao MEC (após a seleção do botão "Enviar" presente na tela "Plano do Município").
    define('PAR3_FLUXO_FPM_REABERTO', 2243);            //O Secretário Estadual reabriu plano (após a seleção do botão "Editar" presente na tela "Plano do Município").

    #ACOES
    define('WF_AEDID_FPM_NAOINICIADO_EMCADASTRAMENTO', 5512);            //O Secretário Municipal aceitou o termo
    define('WF_AEDID_FPM_NAOINICIADO_NAOACEITOUOTERMO', 5513);           //O Secretário Municipal naõ aceitou o termo.
    define('WF_AEDID_FPM_EMCADSTRAMENTO_REABRIR', 5514);                 //O Secretário Municipal naõ aceitou o termo.
    define('WF_AEDID_FPM_REABERTO_EMCADASTRAMENTO', 5515);            //O Secretário Municipal enviou naõ aceitou o termo.
    define('WF_AEDID_FPM_REABERTO_ENVIADOAOMEC', 5520);            //O Secretário Municipal enviou naõ aceitou o termo.
    define('WF_AEDID_FPM_NAOACEITOU_EMCADASTRAMENTO', 5517);            //O Secretário Municipal enviou naõ aceitou o termo.

    #Prestação de contas do PAR 2 - Necessários para lista de documentos em Execução e Acompanhamento
    define('ESDID_PC_OBRAS_EM_CADASTRAMENTO', 2174 );
    define('ESDID_OBRAS_SITUACAO_OPC_INADIMPLENTE', 2278 ); /* OPC OBRAS */
    define('ESDID_AGUARDANDO_ENVIO_PROPONENTE', 2005 );
    define('ESDID_OPC_INADIMPLENTE', 2298 );

    //CONSTANTES PAR ANTIGO
    define("PAR_PERFIL_NUTRICIONISTA", 1374);
    
    #Fluxo do Habilita
    define('PAR3_FLUXO_HABILITA', 353);
    define('PAR3_ESD_HABILITA_EM_CADASTRAMENTO', 2354);
    define('PAR3_ESD_HABILITA_AGUARDANDO_DISTRIBUICAO', 2355);
    define('PAR3_ESD_HABILITA_AGUARDANDO_ANALISE', 2380);
    define('PAR3_ESD_HABILITA_EM_ANALISE', 2356);
    define('PAR3_ESD_HABILITA_EM_DILIGENCIA', 2357);
    define('PAR3_ESD_HABILITA_HABILITADO', 2358);
     
    define('PAR3_AEDID_HABILITA_ENVIA_ANALISE', 5585);
    define('WF_AEDID_HABILITA_AGUARDANDODISTRIBUICAO_EMANALISE', 5586);
    define('PAR3_AEDID_HABILITA_ENVIA_DILIGENCIA', 5587);
    define('PAR3_AEDID_HABILITA_ENVIA_DILIGENCIA_ANALISE', 5589);
    define('PAR3_AEDID_HABILITA_ENVIA_ENTIDADE_HABILITADA', 5588);
    /* Aqui classificação da omissão do CACS*/
    define("ROPID_GESTOR_UNIDADE", 1 );
    define("ROPID_PRESIDENTE", 2 );
    define('DPCID_OMISSAO_CACS', 5 );
}

# Tipos de objeto
define('TIPO_OBJETO_OBRA', 1);
?>