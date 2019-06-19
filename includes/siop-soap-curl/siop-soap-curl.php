<?php

# Cliente do Webservice
include_once APPRAIZ. 'includes/soap-curl/client.php';

# Contrato de classes
include_once APPRAIZ. 'includes/siop-soap-curl/interface-service.php';
include_once APPRAIZ. 'includes/siop-soap-curl/interface-xml.php';

# Classes abstratas pra implementa??o de servi?os
include_once APPRAIZ. 'includes/siop-soap-curl/service.php';
include_once APPRAIZ. 'includes/siop-soap-curl/xml.php';

# Receita
include_once APPRAIZ. 'includes/siop-soap-curl/receita/service/captar-base-externa.php';
include_once APPRAIZ. 'includes/siop-soap-curl/receita/xml/captar-base-externa.php';

# Qualitativo
include_once APPRAIZ. 'includes/siop-soap-curl/qualitativo/service/programacao-completa.php';
include_once APPRAIZ. 'includes/siop-soap-curl/qualitativo/xml/programacao-completa.php';

# Quantitativo
include_once APPRAIZ. 'includes/siop-soap-curl/quantitativo/service.php';
include_once APPRAIZ. 'includes/siop-soap-curl/quantitativo/xml.php';
