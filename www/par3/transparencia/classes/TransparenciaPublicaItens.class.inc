<?php

class TransparenciaPublicaItens extends Modelo
{
    public static $sqlEstados = 'SELECT estuf AS codigo, estuf || \' - \' || estdescricao AS descricao FROM territorios.estado %s ORDER BY estuf';
    public static $sqlMunicipios = 'SELECT muncod AS codigo, mundescricao AS descricao FROM territorios.municipio WHERE estuf = \'%s\' ORDER BY estuf, mundescricao';
    public static $sqlEnsinoEtapa = 'SELECT etaid AS codigo, etadsc AS descricao FROM par3.ensino_etapa ORDER BY etadsc';
    public static $sqlModalidade = 'SELECT modid AS codigo, moddsc AS descricao FROM par3.modalidade ORDER BY moddsc';
    public static $sqlIniciativaTipoObjeto = 'SELECT intoid AS codigo, intodsc AS descricao FROM par3.iniciativa_tipos_objeto ORDER BY intodsc';
    public static $sqlRegioes = 'SELECT regcod AS codigo, regdescricao AS descricao FROM territorios.regiao ORDER BY regdescricao';

    /**
     * @param $estuf
     * @return bool|mixed|NULL|string
     */
    public function buscaRegiaoPorEstado($estuf)
    {
        $sqlRegiaoPorEstado = sprintf('
          SELECT 
            reg.regcod
          FROM 
            territorios.regiao reg 
            INNER JOIN territorios.estado est ON reg.regcod = est.regcod 
          WHERE 
            est.estuf = \'%s\'
        ', $estuf);

        return $this->pegaUm($sqlRegiaoPorEstado);
    }

    /**
     * @param array $filters
     */
    public function montarListagemTransparenciaV1(array $filters = [])
    {
        $sql = '
            SELECT 
              uf,
              mundescricao,
              numero_processo,
              numero_documento,
              tipo_documento,
              situacao_documento,
              TO_CHAR(data_vigencia_documento, \'DD/MM/YYYY\') AS data_vigencia_documento,
              valor_planejado_total_iniciativa,
              valor_empenhado,
              valor_aprovado,
              valor_saldo
            FROM 
              par3.vm_transparencia_publica_itens
        ';

        if (array_key_exists('itrid', $filters) && $filters['itrid'] === 'T') {
            unset($filters['itrid']);
        }

        $dataAttributes = array_map(function ($value, $key) {
            if (!empty($value)) {

                if (is_int($value)) {
                    return $key . '=' . trim($value) . '';
                }

                if (is_string($value)) {
                    return $key . '=\'' . trim($value) . '\'';
                }

            }
        }, array_values($filters), array_keys($filters));

        $whereValues = array_filter($dataAttributes, 'strlen');

        if (!empty($whereValues)) {
            $sql .= ' WHERE ' . implode(' AND ', $whereValues);
        }

        $sql .= ' ORDER BY uf ASC';

        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = [
            'UF',
            'Município',
            'Nº do Processo',
            'Nº do Documento',
            'Tipo de Documento',
            'Situação',
            'Data da Vigência',
            'Valor do Termo',
            'Valor Empenhado',
            'Pagamento Efetivado',
            'Saldo Bancário (CC + CP + Fundo)',
        ];

        $listagemSimec->setCabecalho($arrayCabecalho)
            ->addCallbackDeCampo('valor_planejado_total_iniciativa', 'mascaraReal')
            ->addCallbackDeCampo('valor_empenhado', 'mascaraReal')
            ->addCallbackDeCampo('valor_aprovado', 'mascaraReal')
            ->addCallbackDeCampo('valor_saldo', 'mascaraReal')
            ->addCallbackDeCampo('processo','formata_numero_processo')
            ->addCallbackDeCampo('termo','formata_Tipo_Documento')
            ->setQuery($sql)
            ->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS)
            ->setTamanhoPagina(100)
            ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }

    public function buscarOnibusPlanejadosModeloAno(array $filter = [])
    {
        $sql = '
            SELECT
              descricao,
              ano,
              COUNT(descricao) AS total
            FROM
              par3.vm_transparencia_publica_itens
            WHERE
              intoid = 4
              AND tipo_veiculo = \'Ônibus\'
        ';

        if (!empty($filter)) {
            $sql .= ' AND ' . implode(' AND ', $filter);
        }

        $sql .= '
            GROUP BY
              descricao,
              ano
            ORDER BY
              descricao, ano
        ';

        return $this->carregar($sql);
    }

    public function buscarOnibusPlanejadosModalidadeAno(array $filter = [])
    {
        $sql = '
            SELECT
              modalidade,
              ano,
              COUNT(modalidade) AS total
            FROM
              par3.vm_transparencia_publica_itens
            WHERE
              intoid = 4
              AND tipo_veiculo = \'Ônibus\'
        ';

        if (!empty($filter)) {
            $sql .= ' AND ' . implode(' AND ', $filter);
        }

        $sql .= '
            GROUP BY
              ano,
              modalidade
            ORDER BY
              modalidade, ano
        ';

        return $this->carregar($sql);
    }

    public function buscarOnibusPlanejadosModeloEtapa(array $filter = [])
    {
        $sql = '
            SELECT
              descricao,
              etapa,
              COUNT(etapa) AS total
            FROM
              par3.vm_transparencia_publica_itens
            WHERE
              intoid = 4
              AND tipo_veiculo = \'Ônibus\'
        ';

        if (!empty($filter)) {
            $sql .= ' AND ' . implode(' AND ', $filter);
        }

        $sql .= '
            GROUP BY
              descricao,
              etapa
            ORDER BY
              etapa, descricao
        ';

        return $this->carregar($sql);
    }

    public function buscarOnibusPlanejadosModeloRegiao(array $filter = [])
    {
        $sql = '
            SELECT
              descricao,
              regiao,
              COUNT(descricao) AS total
            FROM
              par3.vm_transparencia_publica_itens
            WHERE
              intoid = 4
              AND tipo_veiculo = \'Ônibus\'
        ';

        if (!empty($filter)) {
            $sql .= ' AND ' . implode(' AND ', $filter);
        }

        $sql .= '
            GROUP BY
              descricao,
              regiao
            ORDER BY
              regiao, descricao
        ';

        return $this->carregar($sql);
    }

    public function buscarOnibusPlanejadosRegiao(array $filter = [])
    {
        $sql = '
            SELECT
              regiao,
              COUNT(regiao) AS total
            FROM
              par3.vm_transparencia_publica_itens
            WHERE
              intoid = 4
              AND tipo_veiculo = \'Ônibus\'
        ';

        if (!empty($filter)) {
            $sql .= ' AND ' . implode(' AND ', $filter);
        }

        $sql .= '
            GROUP BY
              regiao
            ORDER BY
              regiao
        ';

        return $this->carregar($sql);
    }

    public function montaHtmlTermo( $texto = '' ){

        $html = '<html>
						<head>
							<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
							<meta http-equiv="Cache-Control" content="no-cache">
							<meta http-equiv="Pragma" content="no-cache">
							<meta http-equiv="Expires" content="-1">
							<title>Termo de Compromisso</title>
							<style type="text/css" media="print">									
								@media print {
									.notprint { display: none }
								 	 html, body {
								        margin: 0;
								        padding: 0;
								        border: 0;
								    }
								    #printable {
								        margin: 0;
								        padding: 0;
								        border: 0;
								        font-size: 14px;
								    }
								    #printable ~ * {
								        display: none;
								    }
								}
							</style>							
						</head>
						<body>
						<div id="topo">
							<table width="100%" style="position:static;">
								<thead>
									<tr>
										<th class="noPadding" align="left">
										<table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug">
											<tr bgcolor="#ffffff">
												<td valign="top" align="center" width="100%"><img src="http://simec.mec.gov.br/imagens/brasao.gif" width="45" height="45" border="0"></td>
											</tr>
											<tr>
												<td colspan="2" align="center" valign="top" style="padding:0 0 5px 0;" bgcolor="#ffffff">
													<b><font style="font-size:11px;font-family:arial;color:#9F9F9F;">MINISTÉRIO DA EDUCAÇÃO<br />FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO</font></b>
												</td>
											</tr>
										</table>
										</th>
									</tr>
								</thead>
								<tbody>
									<tr>
										<td width="100%">
										<input type="hidden" name="dotid" id="dotid" value="">
											<div id="div_conteudo_termo" style="font-weight:normal;vertical-align:top; FONT: 8pt Arial,verdana; width: 100%">'.$texto.'</div>
										</td>
									</tr>
								</tbody>
							</table>
						</div>
						
					</body>
				</html>';
        return $html;
    }
}
